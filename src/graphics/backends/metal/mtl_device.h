#ifndef BORED_ENGINE_MTL_DEVICE_H
#define BORED_ENGINE_MTL_DEVICE_H

#include "../../device.h"

#import <Metal/MTLDevice.h>

namespace graphics
{
	class MetalDevice final : public IDevice
	{
	public:
		explicit MetalDevice();
		~MetalDevice() override;

	private:
		id<MTLDevice> device;
	};
}

#endif //BORED_ENGINE_MTL_DEVICE_H
