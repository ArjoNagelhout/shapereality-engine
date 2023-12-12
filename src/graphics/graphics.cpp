#include <cassert>
#include "graphics.h"
#include "device.h"
#include "backends/metal/mtl_device.h"
#include "backends/vulkan/vk_device.h"

namespace graphics
{
	std::unique_ptr<IDevice> createDevice(GraphicsBackend const& backend)
	{
		switch (backend)
		{
			case GraphicsBackend::Metal:
				return std::make_unique<MetalDevice>();
			case GraphicsBackend::Vulkan:
				return std::make_unique<VulkanDevice>();
			default:
				return nullptr;
		}
	}
}