#include "application.h"

#include <iostream>

#include "renderer/backends/metal/mtl_renderer.h"
#include "renderer/backends/vulkan/vk_renderer.h"

// common implementation between platforms

namespace engine
{
	ApplicationDelegate::~ApplicationDelegate() = default;

	void ApplicationDelegate::applicationDidFinishLaunching()
	{
	}

	ApplicationDelegate* Application::getDelegate()
	{
		return pDelegate;
	}

	void Application::setDelegate(engine::ApplicationDelegate* delegate)
	{
		pDelegate = delegate;
	}

	renderer::Renderer* Application::getRenderer()
	{
		return pRenderer.get();
	}
}