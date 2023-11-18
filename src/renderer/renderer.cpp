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

	RendererDelegate::~RendererDelegate() = default;

	void RendererDelegate::render(engine::Window* window)
	{
	}

	Renderer::~Renderer() = default;

	RendererDelegate* Renderer::getDelegate()
	{
		return pDelegate;
	}

	void Renderer::setDelegate(renderer::RendererDelegate* delegate)
	{
		pDelegate = delegate;
	}

	void Renderer::addWindow(engine::Window* window)
	{
	}

	void Renderer::removeWindow(engine::Window* window)
	{
	}

	std::unique_ptr<Texture> Renderer::createTexture()
	{
		return std::make_unique<Texture>(TextureFormat::Undefined);
	}

	std::unique_ptr<Material> Renderer::createMaterial()
	{
		return std::make_unique<Material>();
	}

	std::unique_ptr<Mesh> Renderer::createMesh()
	{
		return std::make_unique<Mesh>();
	}

	std::unique_ptr<Shader> Renderer::createShader()
	{
		return std::make_unique<Shader>();
	}
}