#include "../vk_renderer.h"

#include <iostream>

namespace renderer
{
	VulkanRendererBackend::VulkanRendererBackend(Renderer* renderer) : RendererBackend(renderer)
	{
	}

	VulkanRendererBackend::~VulkanRendererBackend() = default;
}