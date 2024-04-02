//
// Created by Arjo Nagelhout on 05/12/2023.
//

#ifndef SHAPEREALITY_DEVICE_H
#define SHAPEREALITY_DEVICE_H

#include <memory>
#include <filesystem>

namespace graphics
{
    struct WindowDescriptor;

    class Window;

    struct CommandQueueDescriptor;

    class ICommandQueue;

    struct RenderPipelineDescriptor;

    class IRenderPipelineState;

    struct DepthStencilDescriptor;

    class IDepthStencilState;

    class IShaderLibrary;

    struct BufferDescriptor;

    class Buffer;

    struct TextureDescriptor;

    class ITexture;

    class IDevice
    {
    public:
        virtual ~IDevice() = default;

        [[nodiscard]] virtual std::unique_ptr<Window>
        createWindow(WindowDescriptor const& descriptor) const = 0;

        [[nodiscard]] virtual std::unique_ptr<ICommandQueue>
        createCommandQueue(CommandQueueDescriptor const& descriptor) const = 0;

        [[nodiscard]] virtual std::unique_ptr<IRenderPipelineState>
        createRenderPipelineState(RenderPipelineDescriptor const& descriptor) const = 0;

        [[nodiscard]] virtual std::unique_ptr<IDepthStencilState>
        createDepthStencilState(DepthStencilDescriptor const& descriptor) const = 0;

        // creates a shader library containing compiled shader source code
        [[nodiscard]] virtual std::unique_ptr<IShaderLibrary>
        createShaderLibrary(std::filesystem::path const& path) const = 0;

        /**
         *
         * @param descriptor
         * @param data pointer to memory on CPU that is exactly the size of the buffer,
         * if nullptr, the buffer needs to be populated using a method on the buffer.
         * @param take createBuffer is now responsible for freeing the memory of `source`.
         * this could remove the need for copying the data for shared or managed buffers (when CPU access is required)
         * if no CPU access is required, there's no advantage to setting this to true, but createBuffer will still
         * deallocate the memory of `data`
         * @return
         */
        [[nodiscard]] virtual std::unique_ptr<Buffer>
        createBuffer(BufferDescriptor const& descriptor, void* source, bool take) const = 0;

        // create buffer without data
        [[nodiscard]] virtual std::unique_ptr<Buffer>
        createBuffer(BufferDescriptor const& descriptor) const = 0;

        [[nodiscard]] virtual std::unique_ptr<ITexture>
        createTexture(TextureDescriptor const& descriptor) const = 0;

        // get a dedicated command queue that we use for transferring data from CPU to GPU
        // this is not a thing in Metal, but in Vulkan and DirectX this concept does exist
        // https://gpuopen.com/learn/concurrent-execution-asynchronous-queues/
        // right now, there's exactly one per device.
        [[nodiscard]] virtual ICommandQueue* transferCommandQueue() const = 0;
    };

    // main entrypoint for creating a graphics device
    [[nodiscard]] std::unique_ptr<IDevice> createDevice();
}

#endif //SHAPEREALITY_DEVICE_H
