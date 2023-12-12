//
// Created by Arjo Nagelhout on 05/12/2023.
//

#ifndef BORED_ENGINE_DEVICE_H
#define BORED_ENGINE_DEVICE_H

#include <memory>

namespace graphics
{
	struct WindowDescriptor;
	class Window;

	struct CommandQueueDescriptor;
	class ICommandQueue;

	struct CommandBufferDescriptor;
	class ICommandBuffer;

	struct BufferDescriptor;
	class IBuffer;

	struct TextureDescriptor;
	class ITexture;

	class IFrameBuffer;

	struct RenderPassDescriptor;
	class IRenderPass;

	struct RenderPipelineDescriptor;
	class IRenderPipelineState;

	class ILibrary;

	class IDevice
	{
	public:
		virtual ~IDevice() = default;

		//
		[[nodiscard]] virtual std::unique_ptr<Window> createWindow(WindowDescriptor descriptor) const;

		//
		[[nodiscard]] virtual std::unique_ptr<ICommandQueue> createCommandQueue(CommandQueueDescriptor descriptor) const;

		//
		[[nodiscard]] virtual std::unique_ptr<IRenderPass> createRenderPass(RenderPassDescriptor const& descriptor) const;

		//
		[[nodiscard]] virtual std::unique_ptr<IRenderPipelineState> createRenderPipelineState(RenderPipelineDescriptor const& descriptor) const;

		//
		[[nodiscard]] virtual std::unique_ptr<ILibrary> createLibrary() const;

//		virtual std::unique_ptr<IBuffer> createBuffer();
//
//		virtual std::unique_ptr<ITexture> createTexture();
//
//		virtual std::unique_ptr<IFrameBuffer> createFrameBuffer();
//
//
//		virtual std::unique_ptr<IShader> createShader();
	};
}

#endif //BORED_ENGINE_DEVICE_H
