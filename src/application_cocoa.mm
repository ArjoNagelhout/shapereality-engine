#include "application.h"

#include <iostream>

#import <Cocoa/Cocoa.h>

#import <Cocoa/Cocoa.h>

// this delegate is the Objective-C implementation of NSApplicationDelegate so that
// we can receive events from the NSApplication instance.
@interface Delegate : NSObject<NSApplicationDelegate>
    @property (unsafe_unretained, nonatomic) engine::Application* pApplication;
@end

@implementation Delegate

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

namespace engine
{
	struct Application::Implementation
	{
		NSApplication* pSharedApplication;
		Delegate* pDelegate;
	};

	Application::Application()
	{
		pRenderer = std::make_unique<renderer::Renderer>();
		pImplementation = std::make_unique<Implementation>();

		// create delegate
		pImplementation->pDelegate = [[Delegate alloc] init];
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

		pImplementation.reset();
		pRenderer.reset();
	}

	void Application::run()
	{
		[pImplementation->pSharedApplication run];
	}
}