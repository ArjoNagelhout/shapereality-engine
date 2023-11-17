#include "application.h"

#include <iostream>

#import <Cocoa/Cocoa.h>

#import <Cocoa/Cocoa.h>

@interface Delegate : NSObject<NSApplicationDelegate>
    @property (unsafe_unretained, nonatomic) engine::Application* pApplication;
@end

@implementation Delegate

- (void)applicationWillFinishLaunching:(NSNotification *)notification
{
	std::cout << "application will finish launching yay" << std::endl;
	return;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification
{
	std::cout << "application did finish launching" << std::endl;

	auto* pApp = (NSApplication*)notification.object;
	[pApp activateIgnoringOtherApps:true];

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
	}

	Application::~Application()
	{
		[pImpl->delegate release];
		[pImpl->pSharedApplication release];

		pImpl.reset();
	}

	void Application::run()
	{
		[pImpl->pSharedApplication run];
	}
}