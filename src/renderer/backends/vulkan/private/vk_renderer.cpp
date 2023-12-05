#include "../vk_graphics.h"

#include <iostream>

namespace renderer
{
	VulkanGraphicsBackend::VulkanGraphicsBackend(Graphics* renderer) : GraphicsBackend(renderer)
	{
	}

	VulkanGraphicsBackend::~VulkanGraphicsBackend() = default;
}