#include "application.h"

#include <iostream>

#include "renderer/backends/metal/mtl_renderer.h"
#include "renderer/backends/vulkan/vk_renderer.h"

// common implementation between platforms

namespace engine
{
	std::vector<std::unique_ptr<Window>>& Application::getWindows()
	{
		return pWindows;
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

	void Application::run()
	{
		while (true)
		{
			pRenderer->render();
		}
	}
}