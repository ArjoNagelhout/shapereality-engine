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

		[[nodiscard]] std::unique_ptr<IWindow> createWindow(WindowDescription description) const override;

		[[nodiscard]] std::unique_ptr<ICommandQueue> createCommandQueue(graphics::CommandQueueDescription description) const override;

	private:
		id<MTLDevice> pDevice;
	};
}

#endif //BORED_ENGINE_MTL_DEVICE_H
