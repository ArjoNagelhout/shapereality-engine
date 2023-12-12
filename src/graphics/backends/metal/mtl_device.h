#ifndef BORED_ENGINE_MTL_DEVICE_H
#define BORED_ENGINE_MTL_DEVICE_H

#include "../../device.h"
#import "../../window.h"

#import <Metal/MTLDevice.h>

namespace graphics
{
	class MetalDevice final : public IDevice
	{
	public:
		explicit MetalDevice();
		~MetalDevice() override;

		[[nodiscard]] std::unique_ptr<Window> createWindow(WindowDescriptor descriptor) const override;

		[[nodiscard]] std::unique_ptr<ICommandQueue> createCommandQueue(CommandQueueDescriptor descriptor) const override;

		[[nodiscard]] std::unique_ptr<IRenderPass> createRenderPass(RenderPassDescriptor const& descriptor) const override;

		[[nodiscard]] std::unique_ptr<IRenderPipelineState> createRenderPipelineState(const graphics::RenderPipelineDescriptor &descriptor) const override;

		[[nodiscard]] std::unique_ptr<IShaderLibrary> createLibrary() const override;

	private:
		id<MTLDevice> pDevice;
	};
}

#endif //BORED_ENGINE_MTL_DEVICE_H
