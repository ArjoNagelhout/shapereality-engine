#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION

#include "metal_renderer.h"

#include <iostream>

namespace renderer
{
	MetalRenderer::MetalRenderer() = default;

	MetalRenderer::~MetalRenderer() = default;

	void MetalRenderer::render()
	{
		std::cout << "metal renderer weee" << std::endl;
	}
}