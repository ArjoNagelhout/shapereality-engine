//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "mtl_device.h"

#include <iostream>

#include "mtl_window.h"

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
}