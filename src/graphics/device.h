//
// Created by Arjo Nagelhout on 05/12/2023.
//

#ifndef BORED_ENGINE_DEVICE_H
#define BORED_ENGINE_DEVICE_H

#include <memory>

namespace graphics
{
	struct WindowDescription;
	class IWindow;

	struct CommandQueueDescription;
	class ICommandQueue;

	struct CommandBufferDescription;
	class ICommandBuffer;

	struct BufferDescription;
	class IBuffer;

	struct TextureDescription;
	class ITexture;

	class IFrameBuffer;
	class IRenderPipeline;
	class IRenderPass;
	class IShader;

	class IDevice
	{
	public:
		virtual ~IDevice() = default;

		[[nodiscard]] virtual std::unique_ptr<IWindow> createWindow(WindowDescription description) const;

		[[nodiscard]] virtual std::unique_ptr<ICommandQueue> createCommandQueue(CommandQueueDescription description) const;

		[[nodiscard]] virtual std::unique_ptr<ICommandBuffer> createCommandBuffer(CommandBufferDescription description) const;
//
//		virtual std::unique_ptr<IBuffer> createBuffer();
//
//		virtual std::unique_ptr<ITexture> createTexture();
//
//		virtual std::unique_ptr<IFrameBuffer> createFrameBuffer();
//
//		virtual std::unique_ptr<IRenderPipeline> createRenderPipeline();
//
//		virtual std::unique_ptr<IRenderPass> createRenderPass();
//
//		virtual std::unique_ptr<IShader> createShader();
	};
}

#endif //BORED_ENGINE_DEVICE_H
