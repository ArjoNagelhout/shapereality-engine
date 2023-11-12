//#include "window_test.h"
//
//#import <Cocoa/Cocoa.h>
//
//class WindowTest::Implementation
//{
//public:
//	NSWindow* pWindow;
//};
//
//WindowTest::WindowTest()
//{
//	pImpl = std::make_unique<Implementation>();
//
//	pImpl->pWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 100, 300)
//									   styleMask:NSWindowStyleMaskTitled
//										 backing:NSBackingStoreBuffered
//										   defer:NO];
//	[pWindow retain];
//	[pWindow makeKeyAndOrderFront:nullptr];
//}
//
//WindowTest::~WindowTest()
//{
//	[pWindow release];
//	pImpl.reset();
//}
//
//WindowTest::SetTitle()
//{
//
//}