#include "cocoa_window.h"

#include <string>
#include <iostream>

#include "cocoa_input.h"
#include "graphics/graphics.h"
#include "graphics/backends/metal/mtl_texture.h"
#include "graphics/backends/metal/mtl_render_pass.h"

@implementation WindowAdapter

@end

@implementation ViewAdapter

- (void)drawRect:(NSRect)dirtyRect {
	_pWindow->getRenderDelegate()->render(_pWindow);
}

- (BOOL)acceptsFirstResponder {
	return YES;
}

- (void)mouseDown:(NSEvent*)event {
	std::cout << "it's the final mouse down. teedeedeeduu, teedeedeeduuduu" << std::endl;
}

- (void)mouseUp:(NSEvent*)event {
	std::cout << "mouse up, up, up. " << std::endl;
}

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
	NSWindowStyleMask toNSWindowStyleMask(WindowFlags_ flags)
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

	std::unique_ptr<Window> createWindow(WindowDescriptor const& descriptor, id <MTLDevice> _Nonnull pDevice)
	{
		// this is put into a separate factory method because we don't want to clutter Window with the MTLDevice argument

		std::unique_ptr<Window> result = std::make_unique<Window>();
		WindowAdapter*& window = result->getImplementation()->pWindowAdapter;
		ViewAdapter*& view = result->getImplementation()->pViewAdapter;

		NSRect rect = NSMakeRect(descriptor.x, descriptor.y, descriptor.width, descriptor.height);

		NSWindowStyleMask mask = toNSWindowStyleMask(static_cast<WindowFlags_>(descriptor.flags));
		window = [[WindowAdapter alloc] initWithContentRect:rect
												  styleMask:mask
													backing:NSBackingStoreBuffered
													  defer:NO];
		[window retain];
		[window makeKeyAndOrderFront:window];
		window.pWindow = result.get();

		view = [[ViewAdapter alloc] initWithFrame:window.frame
										   device:pDevice];
		[view retain];
		view.pWindow = result.get();
		[view setColorPixelFormat:MTLPixelFormatBGRA8Unorm_sRGB];
		Color c{0.f, 0.5f, 1.f, 1.f};
		[view setClearColor:MTLClearColorMake(c.r, c.g, c.b, c.a)];
		[view setDepthStencilPixelFormat:MTLPixelFormatDepth16Unorm];
		[view setClearDepth:1.0f];
		[window setContentView:view];

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

	std::unique_ptr<IRenderPass> Window::getRenderPass() const
	{
		return std::make_unique<MetalRenderPass>(pImplementation->pViewAdapter.currentRenderPassDescriptor);
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

	void Window::fullscreen()
	{
		[pImplementation->pWindowAdapter setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
		[pImplementation->pWindowAdapter toggleFullScreen:nullptr];
	}

	void Window::setPosition(int x, int y)
	{
		[pImplementation->pWindowAdapter setFrameOrigin:NSMakePoint(x, y)];
	}

	void Window::setSize(int width, int height)
	{
		[pImplementation->pWindowAdapter setContentSize:NSMakeSize(width, height)];
	}

	void Window::setMinSize(int width, int height)
	{
		[pImplementation->pWindowAdapter setMinSize:NSMakeSize(width, height)];
	}

	void Window::setMaxSize(int width, int height)
	{
		[pImplementation->pWindowAdapter setMaxSize:NSMakeSize(width, height)];
	}

	math::Rect Window::getRect() const
	{
		CGRect rect = [pImplementation->pWindowAdapter frame];
		return math::Rect{static_cast<float>(rect.origin.x), static_cast<float>(rect.origin.y),
						  static_cast<float>(rect.size.width), static_cast<float>(rect.size.height)};
	}

	void Window::setRect(math::Rect const& rect)
	{
		[pImplementation->pWindowAdapter setFrame:NSMakeRect(rect.x, rect.y, rect.width, rect.height)
										  display:YES
										  animate:NO];
	}
}