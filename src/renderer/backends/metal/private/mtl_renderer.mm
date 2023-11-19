#include "../mtl_renderer.h"
#include "mtl_renderer_implementation.h"

namespace renderer
{
	MetalRendererBackend::MetalRendererBackend(Renderer* renderer) : RendererBackend(renderer)
	{
		pImplementation = std::make_unique<Implementation>();

		pImplementation->pDevice = MTLCreateSystemDefaultDevice();
	}

	MetalRendererBackend::~MetalRendererBackend()
	{
		[pImplementation->pDevice release];
	}

	MetalRendererBackend::Implementation* MetalRendererBackend::getImplementation()
	{
		return pImplementation.get();
	}
}