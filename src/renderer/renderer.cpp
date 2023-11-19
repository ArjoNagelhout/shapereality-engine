#include <cassert>
#include "renderer.h"
#include "backends/metal/mtl_renderer.h"
#include "backends/vulkan/vk_renderer.h"

namespace renderer
{
	std::string ToString(RendererBackendType const &rendererBackend)
	{
		switch (rendererBackend)
		{
			case RendererBackendType::None:
				return "None";
			case RendererBackendType::Metal:
				return "Metal";
			case RendererBackendType::OpenGL:
				return "OpenGL";
			case RendererBackendType::Vulkan:
				return "Vulkan";
			case RendererBackendType::WebGPU:
				return "WebGPU";
		}
	}

	//------------------------------------------------
	//  RendererObject
	//------------------------------------------------

	RendererObject::RendererObject()
	{
		Renderer::pInstance->registerObject(this);
	}

	RendererObject::~RendererObject()
	{
		Renderer::pInstance->unregisterObject(this);
	}

	void RendererObject::onRendererBackendChanged(const renderer::RendererBackendType& rendererBackendType)
	{
	}

	//------------------------------------------------
	//  Renderer
	//------------------------------------------------

	Renderer* Renderer::pInstance{nullptr};

	Renderer::Renderer()
	{
		assert(pInstance == nullptr && "there can only be one renderer");
		pInstance = this;
	}

	Renderer::~Renderer() = default;

	void Renderer::registerObject(renderer::RendererObject* object)
	{
		pObjects.insert(object);
		object->onRendererBackendChanged(rendererBackendType); // on first registering, call its renderer backend changed function so it initializes
	}

	void Renderer::unregisterObject(renderer::RendererObject* object)
	{
		pObjects.erase(object);
	}

	RendererBackendType Renderer::getRendererBackendType()
	{
		return rendererBackendType;
	}

	void Renderer::setRendererBackendType(const renderer::RendererBackendType& _rendererBackendType)
	{
		rendererBackendType = _rendererBackendType;

		switch (rendererBackendType)
		{
			case RendererBackendType::Metal:
				rendererBackend = std::make_unique<MetalRendererBackend>(this);
				break;
			case RendererBackendType::Vulkan:
				rendererBackend = std::make_unique<VulkanRendererBackend>(this);
				break;
			default:
				rendererBackend.reset();
				break;
		}

		// switch implementation for renderer objects to the one for the current renderer backend type
		for (RendererObject* object : pObjects)
		{
			object->onRendererBackendChanged(rendererBackendType);
		}
	}

	RendererBackend* Renderer::getRendererBackend()
	{
		return rendererBackend.get();
	}

	//------------------------------------------------
	//  RendererBackend
	//------------------------------------------------

	RendererBackend::RendererBackend(renderer::Renderer* renderer) : pRenderer(renderer)
	{
	}

	RendererBackend::~RendererBackend() = default;
}