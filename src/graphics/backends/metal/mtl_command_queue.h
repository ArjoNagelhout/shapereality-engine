//
// Created by Arjo Nagelhout on 06/12/2023.
//

#ifndef BORED_ENGINE_MTL_COMMAND_QUEUE_H
#define BORED_ENGINE_MTL_COMMAND_QUEUE_H

#include "../../command_queue.h"

#import <Metal/Metal.h>

namespace graphics
{
	class MetalCommandQueue final : public ICommandQueue
	{
	public:
		explicit MetalCommandQueue(CommandQueueDescription description, id<MTLDevice> _Nonnull pDevice);
		~MetalCommandQueue() override;

	private:
		id<MTLCommandQueue> _Nonnull pCommandQueue;
	};
}

#endif //BORED_ENGINE_MTL_COMMAND_QUEUE_H
