//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_MTL_COMMAND_BUFFER_H
#define BORED_ENGINE_MTL_COMMAND_BUFFER_H

#include "../../command_buffer.h"

namespace graphics
{
	class MetalCommandBuffer final : public CommandBufferImplementation
	{
	public:
		explicit MetalCommandBuffer(CommandBuffer* commandBuffer);
		~MetalCommandBuffer() override;
	};
}

#endif //BORED_ENGINE_MTL_COMMAND_BUFFER_H
