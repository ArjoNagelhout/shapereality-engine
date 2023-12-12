//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "device.h"

#include "window.h"
#include "command_queue.h"
#include "render_pass.h"
#include "render_pipeline_state.h"

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

	std::unique_ptr<IRenderPipelineState>
	IDevice::createRenderPipelineState(const graphics::RenderPipelineDescriptor& descriptor) const
	{
		assert(false && "interface class method should not be called");
	}

	std::unique_ptr<ILibrary> IDevice::createLibrary() const
	{
		assert(false && "interface class method should not be called");
	}
}