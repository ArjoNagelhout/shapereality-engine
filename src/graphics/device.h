//
// Created by Arjo Nagelhout on 05/12/2023.
//

#ifndef BORED_ENGINE_DEVICE_H
#define BORED_ENGINE_DEVICE_H

#include <memory>

namespace graphics
{
	struct WindowDescriptor;
	class IWindow;

	struct CommandQueueDescriptor;
	class ICommandQueue;

	struct CommandBufferDescriptor;
	class ICommandBuffer;

	struct BufferDescriptor;
	class IBuffer;

	struct TextureDescriptor;
	class ITexture;

	class IFrameBuffer;
	class IRenderPipeline;

	struct RenderPassDescriptor;
	class IRenderPass;
	class IShader;

	class IDevice
	{
	public:
		virtual ~IDevice() = default;

		//
		[[nodiscard]] virtual std::unique_ptr<IWindow> createWindow(WindowDescriptor descriptor) const;

		//
		[[nodiscard]] virtual std::unique_ptr<ICommandQueue> createCommandQueue(CommandQueueDescriptor descriptor) const;

		//
		[[nodiscard]] virtual std::unique_ptr<IRenderPass> createRenderPass(RenderPassDescriptor descriptor) const;

//		virtual std::unique_ptr<IBuffer> createBuffer();
//
//		virtual std::unique_ptr<ITexture> createTexture();
//
//		virtual std::unique_ptr<IFrameBuffer> createFrameBuffer();
//
//		virtual std::unique_ptr<IRenderPipeline> createRenderPipeline();
//
//		virtual std::unique_ptr<IShader> createShader();
	};
}

#endif //BORED_ENGINE_DEVICE_H
