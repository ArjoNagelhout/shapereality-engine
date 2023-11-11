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