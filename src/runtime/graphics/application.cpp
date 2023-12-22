#include "application.h"

#include <iostream>

// common implementation between platforms

namespace graphics
{
	void IApplicationDelegate::applicationDidFinishLaunching()
	{
	}

	IApplicationDelegate* Application::getDelegate() const
	{
		return pDelegate;
	}

	void Application::setDelegate(IApplicationDelegate* delegate)
	{
		pDelegate = delegate;
	}
}