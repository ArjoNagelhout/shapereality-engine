//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "device.h"

#include "window.h"
#include "command_queue.h"
#include "render_pass.h"

namespace graphics
{
	std::unique_ptr<Window> IDevice::createWindow(WindowDescriptor descriptor) const
	{
		assert(false && "interface class method should not be called");
	}

	std::unique_ptr<ICommandQueue> IDevice::createCommandQueue(CommandQueueDescriptor descriptor) const
	{
		assert(false && "interface class method should not be called");
	}

	std::unique_ptr<IRenderPass> IDevice::createRenderPass(RenderPassDescriptor const& descriptor) const
	{
		assert(false && "interface class method should not be called");
	}
}