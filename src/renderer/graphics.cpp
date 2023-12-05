#include <cassert>
#include "graphics.h"
#include "backends/metal/mtl_graphics.h"
#include "backends/vulkan/vk_graphics.h"

namespace renderer
{
	std::string ToString(GraphicsBackendType const &graphicsBackendType)
	{
		switch (graphicsBackendType)
		{
			case GraphicsBackendType::None:
				return "None";
			case GraphicsBackendType::Metal:
				return "Metal";
			case GraphicsBackendType::OpenGL:
				return "OpenGL";
			case GraphicsBackendType::Vulkan:
				return "Vulkan";
			case GraphicsBackendType::WebGPU:
				return "WebGPU";
		}
	}

	//------------------------------------------------
	//  GraphicsObject
	//------------------------------------------------

	GraphicsObject::~GraphicsObject()
	{
		Graphics::pInstance->unregisterObject(this);
	}

	void GraphicsObject::onGraphicsBackendChanged(const GraphicsBackendType& rendererBackendType)
	{
	}

	void GraphicsObject::registerObject()
	{
		Graphics::pInstance->registerObject(this);
	}

	//------------------------------------------------
	//  Graphics
	//------------------------------------------------

	Graphics* Graphics::pInstance{nullptr};

	Graphics::Graphics()
	{
		assert(pInstance == nullptr && "there can only be one `Graphics` instance");
		pInstance = this;
	}

	Graphics::~Graphics() = default;

	void Graphics::registerObject(renderer::GraphicsObject* object)
	{
		pObjects.insert(object);
		object->onGraphicsBackendChanged(graphicsBackendType); // on first registering, call its graphics backend changed function so it initializes
	}

	void Graphics::unregisterObject(renderer::GraphicsObject* object)
	{
		pObjects.erase(object);
	}

	GraphicsBackendType Graphics::getGraphicsBackendType() const
	{
		return graphicsBackendType;
	}

	void Graphics::setGraphicsBackendType(const renderer::GraphicsBackendType& type)
	{
		graphicsBackendType = type;

		switch (graphicsBackendType)
		{
			case GraphicsBackendType::Metal:
				graphicsBackend = std::make_unique<MetalGraphicsBackend>(this);
				break;
			case GraphicsBackendType::Vulkan:
				graphicsBackend = std::make_unique<VulkanGraphicsBackend>(this);
				break;
			default:
				graphicsBackend.reset();
				break;
		}

		// switch implementation for graphics objects to the one for the current graphics backend type
		for (GraphicsObject* object : pObjects)
		{
			object->onGraphicsBackendChanged(graphicsBackendType);
		}
	}

	GraphicsBackend* Graphics::getGraphicsBackend()
	{
		return graphicsBackend.get();
	}

	//------------------------------------------------
	//  GraphicsBackend
	//------------------------------------------------

	GraphicsBackend::GraphicsBackend(renderer::Graphics* renderer) : pGraphics(renderer)
	{
	}

	GraphicsBackend::~GraphicsBackend() = default;
}