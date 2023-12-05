#include "../mtl_graphics.h"
#include "mtl_implementation.h"

namespace renderer
{
	MetalGraphicsBackend* MetalGraphicsBackend::pInstance{nullptr};

	MetalGraphicsBackend::MetalGraphicsBackend(Graphics* renderer) : GraphicsBackend(renderer)
	{
		assert(pInstance == nullptr && "There can only be one Metal backend at the same time");
		pInstance = this;

		pImplementation = std::make_unique<Implementation>();

		pImplementation->pDevice = MTLCreateSystemDefaultDevice();
	}

	MetalGraphicsBackend::~MetalGraphicsBackend()
	{
		[pImplementation->pDevice release];

		pInstance = nullptr;
	}

	MetalGraphicsBackend::Implementation* MetalGraphicsBackend::getImplementation()
	{
		return pImplementation.get();
	}
}