#include "vk_renderer.h"

#include <iostream>

namespace renderer
{
	VulkanRenderer::VulkanRenderer()
	{
	}

	VulkanRenderer::~VulkanRenderer() = default;

	void VulkanRenderer::render()
	{
		std::cout << "vulkan pRenderer weee" << std::endl;
	}
}