#include "window_test.h"

#import <Cocoa/Cocoa.h>

WindowTest::WindowTest()
{
	pWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 100, 100)
									   styleMask:NSWindowStyleMaskTitled
										 backing:NSBackingStoreBuffered
										   defer:NO];
	[pWindow retain];
	[pWindow makeKeyAndOrderFront:nullptr];
}

WindowTest::~WindowTest()
{
	[pWindow release];
}