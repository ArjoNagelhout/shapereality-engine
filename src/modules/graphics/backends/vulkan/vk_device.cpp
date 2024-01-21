//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "vk_device.h"

#include <cassert>

namespace graphics
{
    VulkanDevice::VulkanDevice() = default;

    VulkanDevice::~VulkanDevice() = default;

    std::unique_ptr<Window>
    VulkanDevice::createWindow(WindowDescriptor const& descriptor) const
    {
        assert(false && "Vulkan is not implemented");
    }

    std::unique_ptr<ICommandQueue>
    VulkanDevice::createCommandQueue(CommandQueueDescriptor const& descriptor) const
    {
        assert(false && "Vulkan is not implemented");
    }

    std::unique_ptr<IRenderPass>
    VulkanDevice::createRenderPass(RenderPassDescriptor const& descriptor) const
    {
        assert(false && "Vulkan is not implemented");
    }

    std::unique_ptr<IRenderPipelineState>
    VulkanDevice::createRenderPipelineState(RenderPipelineDescriptor const& descriptor) const
    {
        assert(false && "Vulkan is not implemented");
    }

    std::unique_ptr<IDepthStencilState>
    VulkanDevice::createDepthStencilState(DepthStencilDescriptor const& descriptor) const
    {
        assert(false && "Vulkan is not implemented");
    }

    std::unique_ptr<IShaderLibrary>
    VulkanDevice::createShaderLibrary(std::filesystem::path const& path) const
    {
        assert(false && "Vulkan is not implemented");
    }

    std::unique_ptr<IBuffer>
    VulkanDevice::createBuffer(BufferDescriptor const& descriptor) const
    {
        assert(false && "Vulkan is not implemented");
    }

    std::unique_ptr<ITexture>
    VulkanDevice::createTexture(TextureDescriptor const& descriptor) const
    {
        assert(false && "Vulkan is not implemented");
    }
}