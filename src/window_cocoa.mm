#include "window.h"

#import <Cocoa/Cocoa.h>

#include <string>

namespace engine
{
	struct Window::Implementation
	{
		NSWindow* pWindow;
	};

	NSWindowStyleMask toNSWindowStyleMask(WindowFlags_ const& flags)
	{
		NSWindowStyleMask mask{};
		if ((flags & WindowFlags_Borderless) != 0)
		{
			mask |= NSWindowStyleMaskBorderless;
		}
		if ((flags & WindowFlags_Titled) != 0)
		{
			mask |= NSWindowStyleMaskTitled;
		}
		if ((flags & WindowFlags_Closable) != 0)
		{
			mask |= NSWindowStyleMaskClosable;
		}
		if ((flags & WindowFlags_Miniaturizable) != 0)
		{
			mask |= NSWindowStyleMaskMiniaturizable;
		}
		if ((flags & WindowFlags_Resizable) != 0)
		{
			mask |= NSWindowStyleMaskResizable;
		}
		if ((flags & WindowFlags_UnifiedTitleAndToolbar) != 0)
		{
			mask |= NSWindowStyleMaskUnifiedTitleAndToolbar;
		}

		return mask;
	}

	Window::Window(int const& x, int const& y, int const& width, int const& height, int const& flags)
	{
		pImpl = std::make_unique<Implementation>();
		NSWindowStyleMask mask = toNSWindowStyleMask(static_cast<WindowFlags_>(flags));
		pImpl->pWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(x, y, width, height)
													 styleMask:mask
													   backing:NSBackingStoreBuffered
														 defer:NO];
		[pImpl->pWindow retain];
		[pImpl->pWindow makeKeyAndOrderFront:nullptr];
	}

	Window::~Window()
	{
		[pImpl->pWindow release];
		pImpl.reset(); // probably not required
	}

	void Window::setTitle(const std::string& title)
	{
		NSString* s = [NSString stringWithCString:title.c_str()
										 encoding:[NSString defaultCStringEncoding]];
		[pImpl->pWindow setTitle:s];
	}
}