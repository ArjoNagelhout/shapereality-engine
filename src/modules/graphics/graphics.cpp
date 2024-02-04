#include <cassert>
#include "graphics.h"
#include "device.h"
#include "graphics/backends/metal/mtl_device.h"
#include "graphics/backends/vulkan/vk_device.h"

namespace graphics
{
    std::unique_ptr<IDevice> createDevice(GraphicsBackend backend)
    {
        switch (backend)
        {
            case GraphicsBackend::Metal:
                return std::make_unique<metal::MetalDevice>();
            case GraphicsBackend::Vulkan:
                return std::make_unique<vulkan::VulkanDevice>();
            default:
                return nullptr;
        }
    }
}