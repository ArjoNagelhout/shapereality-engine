#ifndef SHAPEREALITY_MTL_DEVICE_H
#define SHAPEREALITY_MTL_DEVICE_H

#include "graphics/device.h"
#include "graphics/window.h"

#import <Metal/MTLDevice.h>

namespace graphics
{
	class MetalDevice final : public IDevice
	{
	public:
		explicit MetalDevice();
		~MetalDevice() override;

		[[nodiscard]] std::unique_ptr<Window> createWindow(WindowDescriptor const& descriptor) const override;

		[[nodiscard]] std::unique_ptr<ICommandQueue> createCommandQueue(CommandQueueDescriptor const& descriptor) const override;

		[[nodiscard]] std::unique_ptr<IRenderPass> createRenderPass(RenderPassDescriptor const& descriptor) const override;

		[[nodiscard]] std::unique_ptr<IRenderPipelineState> createRenderPipelineState(RenderPipelineDescriptor const& descriptor) const override;

		[[nodiscard]] std::unique_ptr<IDepthStencilState> createDepthStencilState(DepthStencilDescriptor const& descriptor) const override;

		[[nodiscard]] std::unique_ptr<IShaderLibrary> createShaderLibrary(std::filesystem::path const& path) const override;

		[[nodiscard]] std::unique_ptr<IBuffer> createBuffer(BufferDescriptor const& descriptor) const override;

	private:
		id<MTLDevice> pDevice;
	};
}

#endif //SHAPEREALITY_MTL_DEVICE_H
