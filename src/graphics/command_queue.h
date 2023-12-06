//
// Created by Arjo Nagelhout on 06/12/2023.
//

#ifndef BORED_ENGINE_COMMAND_QUEUE_H
#define BORED_ENGINE_COMMAND_QUEUE_H

namespace graphics
{
	struct CommandQueueDescription
	{

	};

	class ICommandQueue
	{
	public:
		virtual ~ICommandQueue() = default;
	};
}

#endif //BORED_ENGINE_COMMAND_QUEUE_H
