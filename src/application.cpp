#include "application.h"

#include <iostream>

#include "renderer/backends/metal/mtl_graphics.h"
#include "renderer/backends/vulkan/vk_graphics.h"

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

	renderer::Graphics* Application::getGraphics()
	{
		return pGraphics.get();
	}
}