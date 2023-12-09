//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "mtl_device.h"

#include <iostream>

#include "mtl_window.h"
#import "../../command_queue.h"

#import "mtl_command_queue.h"

namespace graphics
{
	MetalDevice::MetalDevice()
	{
		pDevice = MTLCreateSystemDefaultDevice();

		std::cout << "created metal device" << std::endl;
	}

	MetalDevice::~MetalDevice()
	{
		[pDevice release];
	}

	std::unique_ptr<IWindow> MetalDevice::createWindow(WindowDescription description) const
	{
		return std::make_unique<MetalWindow>(description, pDevice);
	}

	std::unique_ptr<ICommandQueue> MetalDevice::createCommandQueue(graphics::CommandQueueDescription description) const
	{
		return std::make_unique<MetalCommandQueue>(description, pDevice);
	}
}