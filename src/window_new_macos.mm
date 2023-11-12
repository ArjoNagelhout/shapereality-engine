#include "window_new.h"

#import <Cocoa/Cocoa.h>

struct WindowNew::Implementation
{
	int x;
	int y;
	int width;
	int height;
	NSWindow* pWindow;
};

WindowNew::WindowNew(int x, int y, int width, int height)
{
	pImpl = std::make_unique<Implementation>();
	pImpl->x = x;
	pImpl->y = y;
	pImpl->width = width;
	pImpl->height = height;
	pImpl->pWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(x, y, width, height)
									   styleMask:NSWindowStyleMaskTitled
										 backing:NSBackingStoreBuffered
										   defer:NO];
	[pImpl->pWindow retain];
	[pImpl->pWindow makeKeyAndOrderFront:nullptr];
}

void WindowNew::getPosition(int& outX, int& outY)
{
	outX = pImpl->x;
	outY = pImpl->y;
}

WindowNew::~WindowNew()
{
	[pImpl->pWindow release];
	pImpl.reset(); // probably not required
}