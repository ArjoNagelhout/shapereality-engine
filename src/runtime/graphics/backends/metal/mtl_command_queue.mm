//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "mtl_command_queue.h"

#include "mtl_command_buffer.h"

namespace graphics
{
	MetalCommandQueue::MetalCommandQueue(CommandQueueDescriptor descriptor, id<MTLDevice> _Nonnull pDevice)
	{
		pCommandQueue = [pDevice newCommandQueue];
	}

	MetalCommandQueue::~MetalCommandQueue()
	{
		[pCommandQueue release];
	}

	std::unique_ptr<ICommandBuffer> MetalCommandQueue::getCommandBuffer() const
	{
		return std::make_unique<MetalCommandBuffer>(pCommandQueue.commandBuffer);
	}
}