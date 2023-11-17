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

	renderer::RendererBackend Application::getRendererBackend() const
	{
		return rendererBackend;
	}

	void Application::setRendererBackend(renderer::RendererBackend const& rendererBackend)
	{
		this->rendererBackend = rendererBackend;

		// can be moved to pRenderer.cpp
		switch (rendererBackend)
		{
			case renderer::RendererBackend::Metal:
				pRenderer = std::make_unique<renderer::MetalRenderer>();
				break;
			case renderer::RendererBackend::Vulkan:
				pRenderer = std::make_unique<renderer::VulkanRenderer>();
				break;
			default:
				pRenderer.reset();
				break;
		}
	}

	renderer::Renderer* Application::getRenderer()
	{
		return pRenderer.get();
	}
}