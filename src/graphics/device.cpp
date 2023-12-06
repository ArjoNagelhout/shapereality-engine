//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "device.h"

#include "window.h"
#include "command_queue.h"
#include "command_buffer.h"

namespace graphics
{
	std::unique_ptr<IWindow> IDevice::createWindow(WindowDescription description) const
	{
		return {};
	}

	std::unique_ptr<ICommandQueue> IDevice::createCommandQueue(CommandQueueDescription description) const
	{
		return {};
	}

	std::unique_ptr<ICommandBuffer> IDevice::createCommandBuffer(CommandBufferDescription description) const
	{
		return {};
	}
}