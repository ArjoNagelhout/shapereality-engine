//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "window.h"

#include "backends/metal/mtl_window.h"
#include "backends/vulkan/vk_window.h"

namespace renderer
{
	WindowDelegate::~WindowDelegate() = default;

	void WindowDelegate::render(renderer::Window* window)
	{
	}

	WindowRendererImplementation::WindowRendererImplementation(renderer::Window* window) : pWindow(window)
	{
	}

	WindowRendererImplementation::~WindowRendererImplementation() = default;

	void Window::onRendererBackendChanged(RendererBackendType const& rendererBackendType)
	{
		switch (rendererBackendType)
		{
			case RendererBackendType::Metal:
				pRendererImplementation = std::make_unique<MetalWindowImplementation>(this);
				break;
			case RendererBackendType::Vulkan:
				pRendererImplementation = std::make_unique<VulkanWindowImplementation>(this);
				break;
			default:
				pRendererImplementation.reset();
		}
	}

	void Window::setDelegate(renderer::WindowDelegate* delegate)
	{
		pDelegate = delegate;
	}
}