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
		if (flags & WindowFlags_Borderless)
		{

		}
		else if (flags & WindowFlags_Closable)
		{

		}

		return NSWindowStyleMaskBorderless;
	}

	Window::Window(int const& x, int const& y, int const& width, int const& height, WindowFlags_ const& flags)
	{
		pImpl = std::make_unique<Implementation>();
		NSWindowStyleMask mask = toNSWindowStyleMask(flags);
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