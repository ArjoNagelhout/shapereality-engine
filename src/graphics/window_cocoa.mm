#include "window.h"

#include "window_cocoa.h"

#import <Cocoa/Cocoa.h>
#include <string>

namespace graphics
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

	IWindow::IWindow(WindowDescriptor descriptor)
	{
		NSRect rect = NSMakeRect(descriptor.x, descriptor.y, descriptor.width, descriptor.height);
		pImplementation = std::make_unique<Implementation>();
		NSWindowStyleMask mask = toNSWindowStyleMask(static_cast<WindowFlags_>(descriptor.flags));
		pImplementation->pWindow = [[NSWindow alloc] initWithContentRect:rect
															   styleMask:mask
																 backing:NSBackingStoreBuffered
																   defer:NO];
		[pImplementation->pWindow retain];
		[pImplementation->pWindow makeKeyAndOrderFront:pImplementation->pWindow];
	}

	IWindow::~IWindow()
	{
		[pImplementation->pWindow release];
		pImplementation.reset(); // probably not required
	}

	void IWindow::setTitle(const std::string& title)
	{
		NSString* s = [NSString stringWithCString:title.c_str()
										 encoding:[NSString defaultCStringEncoding]];
		[pImplementation->pWindow setTitle:s];
	}

	void IWindow::show()
	{
		[pImplementation->pWindow deminiaturize:nullptr];
	}

	void IWindow::hide()
	{
		[pImplementation->pWindow performMiniaturize:nullptr];
	}

	void IWindow::maximize()
	{
		NSRect frame = [NSScreen mainScreen].frame;
		[pImplementation->pWindow setFrame:frame display:YES animate:YES];
	}

	void IWindow::fullscreen()
	{
		[pImplementation->pWindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
		[pImplementation->pWindow toggleFullScreen:nullptr];
	}

	void IWindow::setPosition(int x, int y)
	{
		[pImplementation->pWindow setFrameOrigin:NSMakePoint(x, y)];
	}

	void IWindow::setSize(int width, int height)
	{
		[pImplementation->pWindow setContentSize:NSMakeSize(width, height)];
	}

	void IWindow::setMinSize(int width, int height)
	{
		[pImplementation->pWindow setMinSize:NSMakeSize(width, height)];
	}

	void IWindow::setMaxSize(int width, int height)
	{
		[pImplementation->pWindow setMaxSize:NSMakeSize(width, height)];
	}

	math::Rect IWindow::getRect() const
	{
		CGRect rect = [pImplementation->pWindow frame];
		return math::Rect{static_cast<float>(rect.origin.x), static_cast<float>(rect.origin.y),
						  static_cast<float>(rect.size.width), static_cast<float>(rect.size.height)};
	}

	void IWindow::setRect(math::Rect const& rect)
	{
		[pImplementation->pWindow setFrame:NSMakeRect(rect.x, rect.y, rect.width, rect.height) display:YES animate:NO];
	}
}