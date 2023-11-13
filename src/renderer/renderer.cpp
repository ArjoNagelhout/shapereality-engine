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

	void Renderer::render()
	{
	}

	void Renderer::addWindow(engine::Window* window)
	{
	}

	void Renderer::removeWindow(engine::Window* window)
	{
	}
}