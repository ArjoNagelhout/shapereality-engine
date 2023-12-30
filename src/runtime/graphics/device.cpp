//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "device.h"

#include "window.h"
#include "command_queue.h"
#include "render_pass.h"
#include "render_pipeline_state.h"

namespace graphics
{
    std::unique_ptr<Window> IDevice::createWindow(WindowDescriptor const& descriptor) const
    {
        assert(false && "interface class method should not be called");
    }

    std::unique_ptr<ICommandQueue> IDevice::createCommandQueue(CommandQueueDescriptor const& descriptor) const
    {
        assert(false && "interface class method should not be called");
    }

    std::unique_ptr<IRenderPass> IDevice::createRenderPass(RenderPassDescriptor const& descriptor) const
    {
        assert(false && "interface class method should not be called");
    }

    std::unique_ptr<IRenderPipelineState>
    IDevice::createRenderPipelineState(RenderPipelineDescriptor const& descriptor) const
    {
        assert(false && "interface class method should not be called");
    }

    std::unique_ptr<IDepthStencilState> IDevice::createDepthStencilState(DepthStencilDescriptor const& descriptor) const
    {
        assert(false && "interface class method should not be called");
    }

    std::unique_ptr<IShaderLibrary> IDevice::createShaderLibrary(std::filesystem::path const& path) const
    {
        assert(false && "interface class method should not be called");
    }

    std::unique_ptr<IBuffer> IDevice::createBuffer(BufferDescriptor const& descriptor) const
    {
        assert(false && "interface class method should not be called");
    }

    std::unique_ptr<ITexture> IDevice::createTexture(TextureDescriptor const& descriptor) const
    {
        assert(false && "interface class method should not be called");
    }
}