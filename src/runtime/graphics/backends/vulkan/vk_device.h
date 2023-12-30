//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef SHAPEREALITY_VK_DEVICE_H
#define SHAPEREALITY_VK_DEVICE_H

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

#endif //SHAPEREALITY_VK_DEVICE_H
