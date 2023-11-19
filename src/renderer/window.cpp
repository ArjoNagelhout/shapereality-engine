//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "window.h"

#include "backends/metal/mtl_window.h"
#include "backends/vulkan/vk_window.h"

namespace renderer
{
	void IWindowDelegate::render(renderer::Window* window)
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
				pRendererImplementation = std::make_unique<MetalWindow>(this);
				break;
			case RendererBackendType::Vulkan:
				pRendererImplementation = std::make_unique<VulkanWindow>(this);
				break;
			default:
				pRendererImplementation.reset();
		}
	}

	IWindowDelegate* Window::getDelegate()
	{
		return pDelegate;
	}

	void Window::setDelegate(renderer::IWindowDelegate* delegate)
	{
		pDelegate = delegate;
	}

	WindowPlatformImplementation* Window::getPlatformImplementation()
	{
		return pPlatformImplementation.get();
	}

	WindowRendererImplementation* Window::getRendererImplementation()
	{
		return pRendererImplementation.get();
	}
}