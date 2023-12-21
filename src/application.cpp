#include "application.h"

#include <iostream>

// common implementation between platforms

namespace engine
{
	void IApplicationDelegate::applicationDidFinishLaunching()
	{
	}

	IApplicationDelegate* Application::getDelegate() const
	{
		return pDelegate;
	}

	void Application::setDelegate(engine::IApplicationDelegate* delegate)
	{
		pDelegate = delegate;
	}
}