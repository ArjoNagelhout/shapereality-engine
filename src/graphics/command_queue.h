//
// Created by Arjo Nagelhout on 06/12/2023.
//

#ifndef BORED_ENGINE_COMMAND_QUEUE_H
#define BORED_ENGINE_COMMAND_QUEUE_H

#include <cassert>
#include <memory>

#include "command_buffer.h"

namespace graphics
{
	struct CommandQueueDescription
	{

	};

	class ICommandQueue
	{
	public:
		virtual ~ICommandQueue() = default;

		//
		virtual std::unique_ptr<ICommandBuffer> getCommandBuffer()
		{
			assert(false && "interface class method should not be called");
		}
	};
}

#endif //BORED_ENGINE_COMMAND_QUEUE_H
