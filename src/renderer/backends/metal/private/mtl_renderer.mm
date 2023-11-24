#include "../mtl_renderer.h"
#include "mtl_implementation.h"

namespace renderer
{
	MetalRendererBackend* MetalRendererBackend::pInstance{nullptr};

	MetalRendererBackend::MetalRendererBackend(Renderer* renderer) : RendererBackend(renderer)
	{
		assert(pInstance == nullptr && "There can only be one Metal backend at the same time");
		pInstance = this;

		pImplementation = std::make_unique<Implementation>();

		pImplementation->pDevice = MTLCreateSystemDefaultDevice();
	}

	MetalRendererBackend::~MetalRendererBackend()
	{
		[pImplementation->pDevice release];

		pInstance = nullptr;
	}

	MetalRendererBackend::Implementation* MetalRendererBackend::getImplementation()
	{
		return pImplementation.get();
	}
}