#include "cocoa_window.h"

#include <string>
#include <iostream>

#include "cocoa_input.h"

@implementation WindowAdapter

- (void)sendEvent:(NSEvent*)event
{
	// todo: make sure we only propagate the events when it
	//       is not being consumed by the default window UIResponder
	//       (which handles resizing and the close, minimize and
	//       fullscreen buttons

	graphics::InputEvent inputEvent = graphics::convert(event);
	if (inputEvent.type != graphics::InputEventType::None)
	{
		_pWindow->getInputDelegate()->onEvent(inputEvent, _pWindow);
	}
	if (inputEvent.type != graphics::InputEventType::Keyboard)
	{
		[super sendEvent:event];
	}
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

	Window::Window(WindowDescriptor descriptor)
	{
		NSRect rect = NSMakeRect(descriptor.x, descriptor.y, descriptor.width, descriptor.height);
		pImplementation = std::make_unique<Implementation>();
		NSWindowStyleMask mask = toNSWindowStyleMask(static_cast<WindowFlags_>(descriptor.flags));
		pImplementation->pWindowAdapter = [[WindowAdapter alloc] initWithContentRect:rect
																		   styleMask:mask
																			 backing:NSBackingStoreBuffered
																			   defer:NO];
		[pImplementation->pWindowAdapter retain];
		[pImplementation->pWindowAdapter makeKeyAndOrderFront:pImplementation->pWindowAdapter];
		pImplementation->pWindowAdapter.pWindow = this;
	}

	Window::~Window()
	{
		[pImplementation->pWindowAdapter release];
		pImplementation.reset(); // probably not required
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
		[pImplementation->pWindowAdapter setFrame:NSMakeRect(rect.x, rect.y, rect.width, rect.height) display:YES animate:NO];
	}
}