#include "graphics/application.h"

#include <iostream>

#import <Cocoa/Cocoa.h>

// this delegate is the Objective-C implementation of NSApplicationDelegate so that
// we can receive events from the NSApplication instance.
@interface NSApplicationDelegate : NSObject<NSApplicationDelegate>
    @property (unsafe_unretained, nonatomic) graphics::Application* pApplication;
@end

@implementation NSApplicationDelegate

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
	auto* pApp = (NSApplication*)notification.object;
	[pApp setActivationPolicy:NSApplicationActivationPolicyRegular];
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
	auto* pApp = (NSApplication*)notification.object;
	[pApp activateIgnoringOtherApps:YES];

	_pApplication->getDelegate()->applicationDidFinishLaunching();
}

@end

namespace graphics
{
	struct Application::Implementation final
	{
		NSApplication* pSharedApplication;
		NSApplicationDelegate* pDelegate;
	};

	Application::Application()
	{
		pImplementation = std::make_unique<Implementation>();

		// create delegate
		pImplementation->pDelegate = [[NSApplicationDelegate alloc] init];
		pImplementation->pDelegate.pApplication = this;

		// create application
		pImplementation->pSharedApplication = [NSApplication sharedApplication];
		[pImplementation->pSharedApplication setDelegate:pImplementation->pDelegate];
	}

	Application::~Application()
	{
		// destroy delegate and application
		[pImplementation->pDelegate release];
		[pImplementation->pSharedApplication release];
	}

	void Application::run()
	{
		[pImplementation->pSharedApplication run];
	}
}