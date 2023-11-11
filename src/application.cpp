#include "application.h"

#include <iostream>

#include "renderer/backends/metal/metal_renderer.h"
#include "renderer/backends/vulkan/vk_renderer.h"

namespace engine
{
	Application::Application() = default;

	Application::~Application() = default;

	renderer::RendererBackend Application::getRendererBackend() const
	{
		return rendererBackend;
	}

	void Application::setRendererBackend(renderer::RendererBackend const& rendererBackend)
	{
		this->rendererBackend = rendererBackend;

		// can be moved to renderer.cpp
		switch (rendererBackend)
		{
			case renderer::RendererBackend::Metal:
				renderer = std::make_unique<renderer::MetalRenderer>();
				break;
			case renderer::RendererBackend::Vulkan:
				renderer = std::make_unique<renderer::VulkanRenderer>();
				break;
			default:
				renderer.reset();
				break;
		}
	}

	void Application::run()
	{
		while (true)
		{
			renderer->render();
		}
	}
}