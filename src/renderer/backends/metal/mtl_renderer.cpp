#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "mtl_renderer.h"

#include <iostream>

namespace renderer
{
	MetalRenderer::MetalRenderer()
	{
		pAutoreleasePool = NS::AutoreleasePool::alloc()->init();
		pDevice = MTL::CreateSystemDefaultDevice();
	}

	MetalRenderer::~MetalRenderer()
	{
		pDevice->release();
		pAutoreleasePool->release();
	}

	MTL::Device* MetalRenderer::getDevice()
	{
		return pDevice;
	}

	void MetalRenderer::addWindow(engine::Window* window)
	{
		// if already exists, remove first
		if (metalViews.contains(window))
		{
			removeWindow(window);
		}

		// create metal view for this window
		metalViews[window] = std::make_unique<MetalView>(this, window);
	}

	void MetalRenderer::removeWindow(engine::Window* window)
	{
		metalViews.erase(window);
	}
}