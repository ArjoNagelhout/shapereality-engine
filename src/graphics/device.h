//
// Created by Arjo Nagelhout on 05/12/2023.
//

#ifndef BORED_ENGINE_DEVICE_H
#define BORED_ENGINE_DEVICE_H

#include <memory>

namespace graphics
{
	class ICommandQueue;
	class ICommandBuffer;
	class IBuffer;
	class ITexture;
	class IFrameBuffer;
	class IRenderPipeline;
	class IRenderPass;
	class IShader;

	class IDevice
	{
	public:
		virtual ~IDevice() = default;

	private:
//		virtual std::unique_ptr<ICommandQueue> createCommandQueue();
//
//		virtual std::unique_ptr<ICommandBuffer> createCommandBuffer();
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
