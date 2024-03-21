#include "cocoa_window.h"

#include <string>
#include <iostream>

#include "cocoa.h"
#include "cocoa_input.h"
#include <graphics/graphics.h>
#include <graphics/backends/metal/mtl_texture.h>
#include <graphics/backends/metal/mtl_render_pass.h>
#include <graphics/backends/metal/mtl_types.h>
#include <common/platform/apple/apple.h>

using namespace graphics;

static void sendEvent(Window* window, graphics::InputEvent const& event)
{
    auto* const inputDelegate = window->getInputDelegate();
    if (inputDelegate)
    {
        inputDelegate->onEvent(event, window);
    }
}

@implementation WindowAdapter

// if we don't set this property, mouse moved events won't be sent by macOS.
- (BOOL)acceptsMouseMovedEvents {
    return YES;
}

- (void)becomeKeyWindow {
    _graphicsWindow->isKeyWindow = true;
    [super becomeKeyWindow];
}

- (void)resignKeyWindow {
    _graphicsWindow->isKeyWindow = false;
    [super resignKeyWindow];
}

@end

@implementation ViewAdapter

// MTKView implementation

- (void)drawRect:(NSRect)dirtyRect {
    _graphicsWindow->getRenderDelegate()->render(_graphicsWindow);
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)acceptsFirstMouse {
    return YES;
}

// NSResponder implementation: Mouse events

- (void)mouseDown:(NSEvent*)event {
    sendEvent(_graphicsWindow, graphics::createMouseEvent(event, MouseEventType::Down, MouseButton::Left));
}

- (void)mouseUp:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Up, MouseButton::Left));
}

- (void)rightMouseDown:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Down, MouseButton::Right));
}

- (void)rightMouseUp:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Up, MouseButton::Right));
}

- (void)otherMouseDown:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Down, MouseButton::Middle));
}

- (void)otherMouseUp:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Up, MouseButton::Middle));
}

- (void)mouseDragged:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Dragged, MouseButton::Left));
}

- (void)rightMouseDragged:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Dragged, MouseButton::Right));
}

- (void)otherMouseDragged:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Dragged, MouseButton::Middle));
}

- (void)mouseMoved:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Moved, MouseButton::None));
}

- (void)mouseEntered:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Entered, MouseButton::None));
}

- (void)mouseExited:(NSEvent*)event {
    sendEvent(_graphicsWindow, createMouseEvent(event, MouseEventType::Exited, MouseButton::None));
}

// NSResponder implementation: Scroll events

- (void)scrollWheel:(NSEvent*)event {
    sendEvent(_graphicsWindow, createScrollEvent(event));
}

// NSResponder implementation: Keyboard events

- (void)keyUp:(NSEvent*)event {
    sendEvent(_graphicsWindow, createKeyboardEvent(event, KeyboardEventType::Up));
}

- (void)keyDown:(NSEvent*)event {
    sendEvent(_graphicsWindow, createKeyboardEvent(event, KeyboardEventType::Down));

    auto* text = _graphicsWindow->getImplementation()->textInputView;
    if (text)
    {
        [text interpretKeyEvents:[NSArray arrayWithObject:event]];
    }
}

- (void)flagsChanged:(NSEvent*)event {
    sendEvent(_graphicsWindow, createKeyboardEvent(event, KeyboardEventType::ModifiersChanged));
}

@end

@implementation TextInputView

- (void)setInputRect:(graphics::Rect const)rect {
    inputRect = rect;
}

// NSTextInputClient implementation

- (BOOL)hasMarkedText {
    return markedText != nullptr;
}

- (NSRange)markedRange {
    return markedRange;
}


- (NSRange)selectedRange {
    return selectedRange;
}

- (NSRect)firstRectForCharacterRange:(NSRange)range actualRange:(NSRangePointer _Nullable)actualRange {

    return NSRect{
        .origin = CGPoint{.x = 100.f, .y = 100.f},
        .size = NSSize{.width = 100.f, .height = 100.f},
    };
}

- (NSUInteger)characterIndexForPoint:(NSPoint)point {
    return 0;
}

- (NSArray<NSAttributedStringKey>* _Nonnull)validAttributesForMarkedText {
    return [[NSArray<NSAttributedStringKey> alloc] init];
}

- (NSAttributedString* _Nullable)attributedSubstringForProposedRange:(NSRange)range actualRange:(NSRangePointer _Nullable)actualRange {
    return nullptr;
}

- (void)insertText:(_Nonnull id)string replacementRange:(NSRange)replacementRange {
    if ([string isKindOfClass:[NSAttributedString class]])
    {
        string = [string string];
    }

    if ([self hasMarkedText])
    {
        [self unmarkText];
    }

    InputEvent textInputEvent(
        TextInputEvent{
            .text = common::toStringUtf8(string)
        }
    );
    std::cout << "insert text" << std::endl;
    sendEvent(_graphicsWindow, textInputEvent);
}


- (void)setMarkedText:(_Nonnull id)string selectedRange:(NSRange)_selectedRange replacementRange:(NSRange)replacementRange {

    // string can be wrapped with attributes
    if ([string isKindOfClass:[NSAttributedString class]])
    {
        string = [string string];
    }

    if ([string length] == 0)
    {
        [self unmarkText];
        return;
    }

    markedText = string;
    selectedRange = _selectedRange;
    markedRange = NSMakeRange(0, [string length]);

    InputEvent textEditingEvent(
        TextEditingEvent{
            .composition = common::toStringUtf8(string),
            .start = static_cast<unsigned int>(_selectedRange.location),
            .length = static_cast<unsigned int>(_selectedRange.length)
        }
    );
    sendEvent(_graphicsWindow, textEditingEvent);
}

- (void)unmarkText {
    markedText = nullptr;

    InputEvent textEditingEvent(
        TextEditingEvent{
            .composition = "",
            .start = 0,
            .length = 0
        }
    );
    _graphicsWindow->getInputDelegate()->onEvent(textEditingEvent, _graphicsWindow);
}

- (NSTextCursorAccessoryPlacement)preferredTextAccessoryPlacement {

}

@end

namespace graphics
{
    NSWindowStyleMask convert(WindowFlags_ flags)
    {
        NSWindowStyleMask mask{};
        mask |= (flags & WindowFlags_Borderless) ? NSWindowStyleMaskBorderless : 0;
        mask |= (flags & WindowFlags_Titled) ? NSWindowStyleMaskTitled : 0;
        mask |= (flags & WindowFlags_Closable) ? NSWindowStyleMaskClosable : 0;
        mask |= (flags & WindowFlags_Miniaturizable) ? NSWindowStyleMaskMiniaturizable : 0;
        mask |= (flags & WindowFlags_Resizable) ? NSWindowStyleMaskResizable : 0;
        mask |= (flags & WindowFlags_UnifiedTitleAndToolbar) ? NSWindowStyleMaskUnifiedTitleAndToolbar : 0;
        return mask;
    }

    std::unique_ptr<Window> createWindow(id <MTLDevice> _Nonnull pDevice, WindowDescriptor const& descriptor)
    {
        // this is put into a separate factory method because we don't want to clutter Window with the MTLDevice argument

        std::unique_ptr<Window> result = std::make_unique<Window>();
        WindowAdapter*& window = result->getImplementation()->windowAdapter;
        ViewAdapter*& view = result->getImplementation()->viewAdapter;

        NSRect rect = NSMakeRect(descriptor.x, descriptor.y, descriptor.width, descriptor.height);

        NSWindowStyleMask mask = convert(static_cast<WindowFlags_>(descriptor.flags));
        window = [[WindowAdapter alloc] initWithContentRect:rect
                                        styleMask:mask
                                        backing:NSBackingStoreBuffered
                                        defer:NO];
        window.graphicsWindow = result.get();

        view = [[ViewAdapter alloc] initWithFrame:window.frame
                                    device:pDevice];
        view.graphicsWindow = result.get();
        [view setColorPixelFormat:MTLPixelFormatBGRA8Unorm_sRGB];
        Color c{0.f, 0.5f, 1.f, 1.f};
        [view setClearColor:metal::convert(c)];
        [view setDepthStencilPixelFormat:MTLPixelFormatDepth16Unorm];
        [view setClearDepth:1.0f];

        [window setContentView:view];
        [window makeFirstResponder:view];
        [window makeKeyAndOrderFront:window];

        [window retain];
        [view retain];

        return result;
    }

    Window::Window()
    {
        implementation = std::make_unique<Implementation>();
    }

    Window::~Window()
    {
        [implementation->viewAdapter release];
        [implementation->windowAdapter release];
        if (implementation->textInputView)
        {
            [implementation->textInputView release];
        }
    }

    std::unique_ptr<ITexture> Window::getDrawable() const
    {
        return std::make_unique<metal::MetalTexture>(implementation->viewAdapter.currentDrawable);
    }

    std::unique_ptr<RenderPassDescriptor> Window::getRenderPassDescriptor() const
    {
        // create a render pass descriptor
        return metal::createRenderPassDescriptor(implementation->viewAdapter.currentRenderPassDescriptor);
    }

    void Window::setTitle(const std::string& title)
    {
        NSString* s = [NSString stringWithCString:title.c_str()
                                encoding:[NSString defaultCStringEncoding]];
        [implementation->windowAdapter setTitle:s];
    }

    void Window::show()
    {
        [implementation->windowAdapter deminiaturize:nullptr];
    }

    void Window::hide()
    {
        [implementation->windowAdapter performMiniaturize:nullptr];
    }

    void Window::maximize()
    {
        NSRect frame = [NSScreen mainScreen].frame;
        [implementation->windowAdapter setFrame:frame display:YES animate:YES];
    }

    void Window::makeFullscreen()
    {
        [implementation->windowAdapter setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
        [implementation->windowAdapter toggleFullScreen:nullptr];
    }

    void Window::makeKeyAndOrderFront()
    {
        [implementation->windowAdapter makeKeyAndOrderFront:implementation->windowAdapter];
    }

    void Window::setPosition(Position position)
    {
        [implementation->windowAdapter setFrameOrigin:convert(position)];
    }

    void Window::setWindowSize(Size size)
    {
        [implementation->windowAdapter setContentSize:convert(size)];
    }

    void Window::setMinSize(Size size)
    {
        [implementation->windowAdapter setMinSize:convert(size)];
    }

    void Window::setMaxSize(Size size)
    {
        [implementation->windowAdapter setMaxSize:convert(size)];
    }

    Rect Window::getWindowRect() const
    {
        return convert([implementation->windowAdapter frame]);
    }

    void Window::setWindowRect(Rect rect)
    {
        [implementation->windowAdapter setFrame:convert(rect)
                                         display:YES
                                         animate:NO];
    }

    Size Window::getContentViewSize() const
    {
        CGRect rect = [implementation->viewAdapter frame];
        return convert(rect.size);
    }

    float Window::getScaleFactor() const
    {
        return static_cast<float>([implementation->windowAdapter backingScaleFactor]);
    }

    void Window::setTextInputRect(graphics::Rect rect)
    {
        auto*& textInput = implementation->textInputView;
        assert(textInput && "error: text input was not first enabled");

        [textInput setInputRect:rect];
    }

    void Window::enableTextInput()
    {
        std::cout << "enable text input" << std::endl;

        auto*& view = implementation->viewAdapter;
        auto*& window = implementation->windowAdapter;
        auto*& textInput = implementation->textInputView;

        // lazy instantiation
        if (!textInput)
        {
            textInput = [[TextInputView alloc] init];
            textInput.graphicsWindow = this;
            [textInput retain];
        }

        if (![textInput.superview isEqual:view])
        {
            [textInput removeFromSuperview];
            [view addSubview:textInput];
            [window makeFirstResponder:textInput];
        }
    }

    // disables text input
    void Window::disableTextInput()
    {
        std::cout << "disable text input" << std::endl;
        auto*& view = implementation->viewAdapter;
        auto*& window = implementation->windowAdapter;
        auto*& textInput = implementation->textInputView;

        if (textInput)
        {
            // important to call makeFirstResponder, otherwise input doesn't get handled properly
            // (from what I understand, TextInputView remains first responder, even though we release it
            // here, resulting in undefined behaviour).
            [window makeFirstResponder:view];
            [textInput removeFromSuperview];
            [textInput release];

            // releasing is not enough! we check for whether the pTextInputView pointer
            // is nullptr, but that doesn't automatically happen on calling release.
            // its memory is simply marked as able to be overridden.
            textInput = nullptr;
        }
    }
}