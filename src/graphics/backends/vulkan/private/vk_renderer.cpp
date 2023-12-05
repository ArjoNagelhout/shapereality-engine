#include "../vk_graphics.h"

#include <iostream>

namespace graphics
{
	VulkanGraphicsBackend::VulkanGraphicsBackend(Graphics* renderer) : GraphicsBackend(renderer)
	{
	}

	VulkanGraphicsBackend::~VulkanGraphicsBackend() = default;
}