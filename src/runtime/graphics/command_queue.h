//
// Created by Arjo Nagelhout on 06/12/2023.
//

#ifndef SHAPEREALITY_COMMAND_QUEUE_H
#define SHAPEREALITY_COMMAND_QUEUE_H

#include <cassert>
#include <memory>

#include "command_buffer.h"

namespace graphics
{
	struct CommandQueueDescriptor
	{

	};

	class ICommandQueue
	{
	public:
		virtual ~ICommandQueue() = default;

		// returns a command buffer object
		[[nodiscard]] virtual std::unique_ptr<ICommandBuffer> getCommandBuffer() const
		{
			assert(false && "interface class method should not be called");
		}
	};
}

#endif //SHAPEREALITY_COMMAND_QUEUE_H
