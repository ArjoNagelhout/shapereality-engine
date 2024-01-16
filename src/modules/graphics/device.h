//
// Created by Arjo Nagelhout on 05/12/2023.
//

#ifndef SHAPEREALITY_DEVICE_H
#define SHAPEREALITY_DEVICE_H

#include <memory>
#include <filesystem>

namespace graphics
{
    //@formatter:off
    struct WindowDescriptor;
    class Window;

    struct CommandQueueDescriptor;
    class ICommandQueue;

    struct RenderPassDescriptor;
    class IRenderPass;

    struct RenderPipelineDescriptor;
    class IRenderPipelineState;

    struct DepthStencilDescriptor;
    class IDepthStencilState;

    class IShaderLibrary;

    struct BufferDescriptor;
    class IBuffer;

    struct TextureDescriptor;
    class ITexture;
    //@formatter:on

    class IDevice
    {
    public:
        virtual ~IDevice() = default;

        //
        [[nodiscard]] virtual std::unique_ptr<Window>
        createWindow(WindowDescriptor const& descriptor) const;

        //
        [[nodiscard]] virtual std::unique_ptr<ICommandQueue>
        createCommandQueue(CommandQueueDescriptor const& descriptor) const;

        //
        [[nodiscard]] virtual std::unique_ptr<IRenderPass>
        createRenderPass(RenderPassDescriptor const& descriptor) const;

        //
        [[nodiscard]] virtual std::unique_ptr<IRenderPipelineState>
        createRenderPipelineState(RenderPipelineDescriptor const& descriptor) const;

        //
        [[nodiscard]] virtual std::unique_ptr<IDepthStencilState>
        createDepthStencilState(DepthStencilDescriptor const& descriptor) const;

        // creates a shader library containing compiled shader source code
        [[nodiscard]] virtual std::unique_ptr<IShaderLibrary>
        createShaderLibrary(std::filesystem::path const& path) const;

        //
        [[nodiscard]] virtual std::unique_ptr<IBuffer>
        createBuffer(BufferDescriptor const& descriptor) const;

        //
        [[nodiscard]] virtual std::unique_ptr<ITexture>
        createTexture(TextureDescriptor const& descriptor) const;
    };
}

#endif //SHAPEREALITY_DEVICE_H
