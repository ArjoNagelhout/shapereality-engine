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
	std::cout << "application will finish launching yay" << std::endl;

	auto* pApp = (NSApplication*)notification.object;
	[pApp setActivationPolicy:NSApplicationActivationPolicyRegular];

	return;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
	std::cout << "application did finish launching" << std::endl;

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
		Delegate* delegate;
	};

	Application::Application()
	{
		pImpl = std::make_unique<Implementation>();
		pImpl->delegate = [[Delegate alloc] init];
		pImpl->delegate.pApplication = this;

		pImpl->pSharedApplication = [NSApplication sharedApplication];
		[pImpl->pSharedApplication setDelegate:pImpl->delegate];

		renderer = std::make_unique<renderer::Renderer>();
	}

	Application::~Application()
	{
		renderer.reset();

		[pImpl->delegate release];
		[pImpl->pSharedApplication release];

		pImpl.reset();
	}

	void Application::run()
	{
		[pImpl->pSharedApplication run];
	}
}