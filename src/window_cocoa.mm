#include "window.h"

#include "view_cocoa.h"

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

	void Window::setContentView(View* view)
	{
		if (view)
		{
			NSView* nsView = view->pImpl->pView;
			[pImpl->pWindow setContentView:nsView];
		}
		else
		{
			// todo: clear content view
			//[pImpl->pWindow setContentView:nullptr];
		}
	}

	void Window::setSize(const int& width, const int& height)
	{

	}

	void Window::setRect(const engine::Rect& rect)
	{

	}

	Rect Window::getRect()
	{
		//[pImpl->pWindow size
		return {};
	}
}