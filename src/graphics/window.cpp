//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "window.h"

#include "backends/metal/mtl_window.h"
#include "backends/vulkan/vk_window.h"

namespace graphics
{
	void IWindowDelegate::render(graphics::Window* window)
	{
	}

	WindowRendererImplementation::WindowRendererImplementation(graphics::Window* window) : pWindow(window)
	{
	}

	WindowRendererImplementation::~WindowRendererImplementation() = default;

	void Window::onGraphicsBackendChanged(GraphicsBackendType const& rendererBackendType)
	{
		switch (rendererBackendType)
		{
			case GraphicsBackendType::Metal:
				pRendererImplementation = std::make_unique<MetalWindow>(this);
				break;
			case GraphicsBackendType::Vulkan:
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

	void Window::setDelegate(graphics::IWindowDelegate* delegate)
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