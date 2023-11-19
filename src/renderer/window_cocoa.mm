#include "window.h"

#include "window_cocoa.h"

#import <Cocoa/Cocoa.h>
#include <string>

namespace renderer
{
	NSWindowStyleMask toNSWindowStyleMask(WindowFlags_ const& flags)
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

	Window::Window(int const& x, int const& y, int const& width, int const& height, int const& flags)
	{
		pPlatformImplementation = std::make_unique<WindowPlatformImplementation>();
		NSWindowStyleMask mask = toNSWindowStyleMask(static_cast<WindowFlags_>(flags));
		pPlatformImplementation->pWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(x, y, width, height)
																	   styleMask:mask
																		 backing:NSBackingStoreBuffered
																		   defer:NO];
		[pPlatformImplementation->pWindow retain];
		[pPlatformImplementation->pWindow makeKeyAndOrderFront:pPlatformImplementation->pWindow];
	}

	Window::~Window()
	{
		[pPlatformImplementation->pWindow release];
		pPlatformImplementation.reset(); // probably not required
	}

	void Window::setTitle(const std::string& title)
	{
		NSString* s = [NSString stringWithCString:title.c_str()
										 encoding:[NSString defaultCStringEncoding]];
		[pPlatformImplementation->pWindow setTitle:s];
	}

	void Window::show()
	{
		[pPlatformImplementation->pWindow deminiaturize:nullptr];
	}

	void Window::hide()
	{
		[pPlatformImplementation->pWindow performMiniaturize:nullptr];
	}

	void Window::maximize()
	{
		NSRect frame = [NSScreen mainScreen].frame;
		[pPlatformImplementation->pWindow setFrame:frame display:YES animate:YES];
	}

	void Window::fullscreen()
	{
		[pPlatformImplementation->pWindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
		[pPlatformImplementation->pWindow toggleFullScreen:nullptr];
	}

	void Window::setPosition(int const& x, int const& y)
	{
		[pPlatformImplementation->pWindow setFrameOrigin:NSMakePoint(x, y)];
	}

	void Window::setSize(int const& width, int const& height)
	{
		[pPlatformImplementation->pWindow setContentSize:NSMakeSize(width, height)];
	}

	void Window::setMinSize(int const& width, int const& height)
	{
		[pPlatformImplementation->pWindow setMinSize:NSMakeSize(width, height)];
	}

	void Window::setMaxSize(int const& width, int const& height)
	{
		[pPlatformImplementation->pWindow setMaxSize:NSMakeSize(width, height)];
	}

	engine::Rect Window::getRect()
	{
		CGRect rect = [pPlatformImplementation->pWindow frame];
		return {static_cast<float>(rect.origin.x), static_cast<float>(rect.origin.y), static_cast<float>(rect.size.width), static_cast<float>(rect.size.height)};
	}

	void Window::setRect(engine::Rect const& rect)
	{
		[pPlatformImplementation->pWindow setFrame:NSMakeRect(rect.x, rect.y, rect.width, rect.height) display:YES animate:NO];
	}
}