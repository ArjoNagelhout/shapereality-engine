#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "../mtl_renderer.h"
#include "../mtl_texture.h"

#include <iostream>

namespace renderer
{
	MetalRendererBackend::MetalRendererBackend(Renderer* renderer) : RendererBackend(renderer)
	{
		pAutoreleasePool = NS::AutoreleasePool::alloc()->init();
		pDevice = MTL::CreateSystemDefaultDevice();
	}

	MetalRendererBackend::~MetalRendererBackend()
	{
		pDevice->release();
		pAutoreleasePool->release();
	}

	MTL::Device* MetalRendererBackend::getDevice()
	{
		return pDevice;
	}
}