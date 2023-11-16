#include "renderer.h"

#include <utility>

namespace renderer
{
	std::string ToString(RendererBackend const &rendererBackend)
	{
		switch (rendererBackend)
		{
			case RendererBackend::None:
				return "None";
			case RendererBackend::Metal:
				return "Metal";
			case RendererBackend::OpenGL:
				return "OpenGL";
			case RendererBackend::Vulkan:
				return "Vulkan";
			case RendererBackend::WebGPU:
				return "WebGPU";
		}
	}

	Renderer::~Renderer() = default;

	void Renderer::addWindow(engine::Window* window)
	{
	}

	void Renderer::removeWindow(engine::Window* window)
	{
	}

	Delegate* Renderer::getDelegate()
	{
		return pDelegate;
	}

	void Renderer::setDelegate(renderer::Delegate* delegate)
	{
		pDelegate = delegate;
	}

	Delegate::~Delegate() = default;

	void Delegate::render(engine::Window* window)
	{
	}
}