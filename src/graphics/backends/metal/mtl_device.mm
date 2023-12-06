//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "mtl_device.h"

#include <iostream>

namespace graphics
{
	MetalDevice::MetalDevice()
	{
		device = MTLCreateSystemDefaultDevice();

		std::cout << "created metal device" << std::endl;
	}

	MetalDevice::~MetalDevice()
	{
		[device release];
	}
}