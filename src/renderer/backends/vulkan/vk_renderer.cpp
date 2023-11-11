#include "vk_renderer.h"

#include <iostream>

namespace renderer
{
	VulkanRenderer::VulkanRenderer() = default;

	VulkanRenderer::~VulkanRenderer() = default;

	void VulkanRenderer::render()
	{
		std::cout << "vulkan renderer weee" << std::endl;
	}
}