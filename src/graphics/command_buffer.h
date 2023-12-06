//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_COMMAND_BUFFER_H
#define BORED_ENGINE_COMMAND_BUFFER_H

namespace graphics
{
	class ICommandBuffer
	{
	public:
		virtual ~ICommandBuffer() = default;
	};
}

#endif //BORED_ENGINE_COMMAND_BUFFER_H
