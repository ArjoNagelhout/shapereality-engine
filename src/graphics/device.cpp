//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "device.h"

#include "window.h"
#include "command_queue.h"
#include "render_pass.h"

namespace graphics
{
	std::unique_ptr<IWindow> IDevice::createWindow(WindowDescriptor descriptor) const
	{
		return {};
	}

	std::unique_ptr<ICommandQueue> IDevice::createCommandQueue(CommandQueueDescriptor descriptor) const
	{
		return {};
	}

	std::unique_ptr<IRenderPass> IDevice::createRenderPass(RenderPassDescriptor descriptor) const
	{
		return {};
	}
}