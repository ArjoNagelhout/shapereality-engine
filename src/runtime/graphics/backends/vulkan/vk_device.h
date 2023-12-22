//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef BORED_ENGINE_VK_DEVICE_H
#define BORED_ENGINE_VK_DEVICE_H

#include "graphics/device.h"

namespace graphics
{
	class VulkanDevice final : public IDevice
	{
	public:
		explicit VulkanDevice();
		~VulkanDevice() override;
	};
}

#endif //BORED_ENGINE_VK_DEVICE_H
