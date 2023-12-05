#include "application.h"

#include <iostream>

#include "graphics/backends/metal/mtl_graphics.h"
#include "graphics/backends/vulkan/vk_graphics.h"

// common implementation between platforms

namespace engine
{
	void IApplicationDelegate::applicationDidFinishLaunching()
	{
	}

	IApplicationDelegate* Application::getDelegate()
	{
		return pDelegate;
	}

	void Application::setDelegate(engine::IApplicationDelegate* delegate)
	{
		pDelegate = delegate;
	}

	graphics::Graphics* Application::getGraphics()
	{
		return pGraphics.get();
	}
}