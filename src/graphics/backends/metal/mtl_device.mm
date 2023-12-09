//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "mtl_device.h"

#include <iostream>

#include "mtl_window.h"
#include "mtl_command_queue.h"
#include "mtl_render_pass.h"

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

	std::unique_ptr<IWindow> MetalDevice::createWindow(WindowDescriptor descriptor) const
	{
		return std::make_unique<MetalWindow>(descriptor, pDevice);
	}

	std::unique_ptr<ICommandQueue> MetalDevice::createCommandQueue(CommandQueueDescriptor descriptor) const
	{
		return std::make_unique<MetalCommandQueue>(descriptor, pDevice);
	}

	std::unique_ptr<IRenderPass> MetalDevice::createRenderPass(RenderPassDescriptor descriptor) const
	{
		return std::make_unique<MetalRenderPass>(descriptor);
	}
}