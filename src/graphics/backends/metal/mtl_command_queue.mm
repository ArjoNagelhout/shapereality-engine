//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "mtl_command_queue.h"

namespace graphics
{
	MetalCommandQueue::MetalCommandQueue(CommandQueueDescription description, id<MTLDevice> _Nonnull pDevice)
	{
		pCommandQueue = [pDevice newCommandQueue];
	}

	MetalCommandQueue::~MetalCommandQueue()
	{
		[pCommandQueue release];
	}
}