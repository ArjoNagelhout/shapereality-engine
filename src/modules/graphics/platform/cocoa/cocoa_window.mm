#include "cocoa_window.h"

#include <string>
#include <iostream>

#include "cocoa.h"
#include "cocoa_input.h"
#include "graphics/graphics.h"
#include "graphics/backends/metal/mtl_texture.h"
#include "graphics/backends/metal/mtl_render_pass.h"
#include "graphics/backends/metal/mtl_types.h"

using namespace graphics;

@implementation WindowAdapter

// if we don't set this property, mouse moved events won't be sent by macOS.
- (BOOL)acceptsMouseMovedEvents {
    return YES;
}

@end

@implementation ViewAdapter

// MTKView implementation

- (void)drawRect:(NSRect)dirtyRect {
    _pWindow->getRenderDelegate()->render(_pWindow);
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)acceptsFirstMouse {
    return YES;
}

- (void)sendEvent:(graphics::InputEvent)event {
    _pWindow->getInputDelegate()->onEvent(event, _pWindow);
}

// NSResponder implementation: Mouse events

- (void)mouseDown:(NSEvent*)event {
    [self sendEvent:graphics::createMouseEvent(event, MouseEventType::Down, MouseButton::Left)];
}

- (void)mouseUp:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Up, MouseButton::Left)];
}

- (void)rightMouseDown:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Down, MouseButton::Right)];
}

- (void)rightMouseUp:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Up, MouseButton::Right)];
}

- (void)otherMouseDown:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Down, MouseButton::Middle)];
}

- (void)otherMouseUp:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Up, MouseButton::Middle)];
}

- (void)mouseDragged:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Dragged, MouseButton::Left)];
}

- (void)rightMouseDragged:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Dragged, MouseButton::Right)];
}

- (void)otherMouseDragged:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Dragged, MouseButton::Middle)];
}

- (void)mouseMoved:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Moved, MouseButton::None)];
}

- (void)mouseEntered:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Entered, MouseButton::None)];
}

- (void)mouseExited:(NSEvent*)event {
    [self sendEvent:createMouseEvent(event, MouseEventType::Exited, MouseButton::None)];
}

// NSResponder implementation: Scroll events

- (void)scrollWheel:(NSEvent*)event {
    [self sendEvent:createScrollEvent(event)];
}

// NSResponder implementation: Keyboard events

- (void)keyUp:(NSEvent*)event {
    [self sendEvent:createKeyboardEvent(event, KeyboardEventType::Up)];
}

- (void)keyDown:(NSEvent*)event {
    [self sendEvent:createKeyboardEvent(event, KeyboardEventType::Down)];
}

- (void)flagsChanged:(NSEvent*)event {
    [self sendEvent:createKeyboardEvent(event, KeyboardEventType::ModifiersChanged)];
}

// NSTextInputClient implementation

- (BOOL)hasMarkedText {
    return NO;
}

- (NSRange)markedRange {
    return NSRange{.location = 0, .length = 0};
}


- (NSRange)selectedRange {
    return NSRange{.location = 0, .length = 0};
}

- (NSRect)firstRectForCharacterRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange {
    return NSRect{
        .origin = CGPoint{.x = 100.f, .y = 100.f},
        .size = NSSize{.width = 100.f, .height = 100.f},
    };
}

- (NSUInteger)characterIndexForPoint:(NSPoint)point {
    return 0;
}

- (nonnull NSArray<NSAttributedStringKey>*)validAttributesForMarkedText {
    return [[NSArray<NSAttributedStringKey> alloc] init];
}

- (nullable NSAttributedString*)attributedSubstringForProposedRange:(NSRange)range actualRange:(nullable NSRangePointer)actualRange {

}

- (void)insertText:(nonnull id)string replacementRange:(NSRange)replacementRange {

}


- (void)setMarkedText:(nonnull id)string selectedRange:(NSRange)selectedRange replacementRange:(NSRange)replacementRange {

}

- (void)unmarkText {

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
        WindowAdapter*& window = result->getImplementation()->pWindowAdapter;
        ViewAdapter*& view = result->getImplementation()->pViewAdapter;

        NSRect rect = NSMakeRect(descriptor.x, descriptor.y, descriptor.width, descriptor.height);

        NSWindowStyleMask mask = convert(static_cast<WindowFlags_>(descriptor.flags));
        window = [[WindowAdapter alloc] initWithContentRect:rect
                                        styleMask:mask
                                        backing:NSBackingStoreBuffered
                                        defer:NO];
        [window retain];
        window.pWindow = result.get();

        view = [[ViewAdapter alloc] initWithFrame:window.frame
                                    device:pDevice];
        [view retain];
        view.pWindow = result.get();
        [view setColorPixelFormat:MTLPixelFormatBGRA8Unorm_sRGB];
        Color c{0.f, 0.5f, 1.f, 1.f};
        [view setClearColor:convert(c)];
        [view setDepthStencilPixelFormat:MTLPixelFormatDepth16Unorm];
        [view setClearDepth:1.0f];

        [window setContentView:view];
        [window makeFirstResponder:view];
        [window makeKeyAndOrderFront:window];

        return result;
    }

    Window::Window()
    {
        pImplementation = std::make_unique<Implementation>();
    }

    Window::~Window()
    {
        [pImplementation->pViewAdapter release];
        [pImplementation->pWindowAdapter release];
    }

    std::unique_ptr<ITexture> Window::getDrawable() const
    {
        return std::make_unique<MetalTexture>(pImplementation->pViewAdapter.currentDrawable);
    }

    std::unique_ptr<RenderPassDescriptor> Window::getRenderPassDescriptor() const
    {
        // create a render pass descriptor
        return createRenderPassDescriptor(pImplementation->pViewAdapter.currentRenderPassDescriptor);
    }

    void Window::setTitle(const std::string& title)
    {
        NSString* s = [NSString stringWithCString:title.c_str()
                                encoding:[NSString defaultCStringEncoding]];
        [pImplementation->pWindowAdapter setTitle:s];
    }

    void Window::show()
    {
        [pImplementation->pWindowAdapter deminiaturize:nullptr];
    }

    void Window::hide()
    {
        [pImplementation->pWindowAdapter performMiniaturize:nullptr];
    }

    void Window::maximize()
    {
        NSRect frame = [NSScreen mainScreen].frame;
        [pImplementation->pWindowAdapter setFrame:frame display:YES animate:YES];
    }

    void Window::makeFullscreen()
    {
        [pImplementation->pWindowAdapter setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
        [pImplementation->pWindowAdapter toggleFullScreen:nullptr];
    }

    void Window::makeKeyAndOrderFront()
    {
        [pImplementation->pWindowAdapter makeKeyAndOrderFront:pImplementation->pWindowAdapter];
    }

    void Window::setPosition(Position position)
    {
        [pImplementation->pWindowAdapter setFrameOrigin:convert(position)];
    }

    void Window::setWindowSize(Size size)
    {
        [pImplementation->pWindowAdapter setContentSize:convert(size)];
    }

    void Window::setMinSize(Size size)
    {
        [pImplementation->pWindowAdapter setMinSize:convert(size)];
    }

    void Window::setMaxSize(Size size)
    {
        [pImplementation->pWindowAdapter setMaxSize:convert(size)];
    }

    Rect Window::getWindowRect() const
    {
        return convert([pImplementation->pWindowAdapter frame]);
    }

    void Window::setWindowRect(Rect rect)
    {
        [pImplementation->pWindowAdapter setFrame:convert(rect)
                                         display:YES
                                         animate:NO];
    }

    Size Window::getContentViewSize() const
    {
        CGRect rect = [pImplementation->pViewAdapter frame];
        return convert(rect.size);
    }

    float Window::getScaleFactor() const
    {
        return static_cast<float>([pImplementation->pWindowAdapter backingScaleFactor]);
    }
}