//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "mtl_device.h"

#include <iostream>

#include "mtl_command_queue.h"
#include "mtl_render_pass.h"
#include "mtl_render_pipeline_state.h"
#include "mtl_shader.h"
#include "mtl_buffer.h"
#include "mtl_texture.h"

#include <graphics/platform/cocoa/cocoa_window.h>
#include <graphics/platform/uikit/uikit_window.h>

namespace graphics
{
    std::unique_ptr<IDevice> createDevice()
    {
        return std::make_unique<metal::MetalDevice>();
    }
}

namespace graphics::metal
{
    MetalDevice::MetalDevice()
    {
        device = MTLCreateSystemDefaultDevice();
        [device retain];
    }

    MetalDevice::~MetalDevice()
    {
        [device release];
    }

    std::unique_ptr<Window>
    MetalDevice::createWindow(WindowDescriptor const& descriptor) const
    {
        return graphics::createWindow(device, descriptor);
    }

    std::unique_ptr<ICommandQueue>
    MetalDevice::createCommandQueue(CommandQueueDescriptor const& descriptor) const
    {
        return std::make_unique<MetalCommandQueue>(device, descriptor);
    }

    std::unique_ptr<IRenderPipelineState>
    MetalDevice::createRenderPipelineState(RenderPipelineDescriptor const& descriptor) const
    {
        return std::make_unique<MetalRenderPipelineState>(device, descriptor);
    }

    std::unique_ptr<IDepthStencilState>
    MetalDevice::createDepthStencilState(DepthStencilDescriptor const& descriptor) const
    {
        return std::make_unique<MetalDepthStencilState>(device, descriptor);
    }

    std::unique_ptr<IShaderLibrary>
    MetalDevice::createShaderLibrary(std::filesystem::path const& path) const
    {
        return std::make_unique<MetalShaderLibrary>(device, path);
    }

    std::unique_ptr<Buffer>
    MetalDevice::createBuffer(BufferDescriptor const& descriptor, void* source, bool take) const
    {
        return std::make_unique<MetalBuffer>(this, descriptor, source, take);
    }

    std::unique_ptr<Buffer>
    MetalDevice::createBuffer(const graphics::BufferDescriptor& descriptor) const
    {
        return std::make_unique<MetalBuffer>(this, descriptor);
    }

    std::unique_ptr<ITexture>
    MetalDevice::createTexture(TextureDescriptor const& descriptor) const
    {
        return std::make_unique<MetalTexture>(device, descriptor);
    }

    ICommandQueue* MetalDevice::transferCommandQueue() const
    {
        static std::unique_ptr<ICommandQueue> instance = createCommandQueue({});
        return instance.get();
    }

    id <MTLDevice> _Nonnull MetalDevice::metalDevice() const
    {
        return device;
    }
}
