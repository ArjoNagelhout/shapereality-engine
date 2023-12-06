//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "mtl_device.h"

namespace graphics
{
	MetalDevice::MetalDevice()
	{
		device = MTLCreateSystemDefaultDevice();
	}

	MetalDevice::~MetalDevice()
	{
		[device release];
	}
}