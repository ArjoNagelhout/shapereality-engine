//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_VK_COMMAND_BUFFER_H
#define BORED_ENGINE_VK_COMMAND_BUFFER_H

#include "../../command_buffer.h"

namespace renderer
{
	class VulkanCommandBuffer final : public CommandBufferImplementation
	{
	public:
		explicit VulkanCommandBuffer(CommandBuffer* commandBuffer);
		~VulkanCommandBuffer() override;
	};
}

#endif //BORED_ENGINE_VK_COMMAND_BUFFER_H
