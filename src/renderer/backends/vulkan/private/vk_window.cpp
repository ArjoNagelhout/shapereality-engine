//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../vk_window.h"

namespace renderer
{
	VulkanWindowImplementation::VulkanWindowImplementation(renderer::Window* window) : WindowRendererImplementation(window)
	{
	}

	VulkanWindowImplementation::~VulkanWindowImplementation() = default;
}