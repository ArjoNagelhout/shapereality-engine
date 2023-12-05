//
// Created by Arjo Nagelhout on 19/11/2023.
//

#include "../vk_command_buffer.h"

namespace graphics
{
	VulkanCommandBuffer::VulkanCommandBuffer(graphics::CommandBuffer* commandBuffer) : CommandBufferImplementation(commandBuffer)
	{
	}

	VulkanCommandBuffer::~VulkanCommandBuffer() = default;
}