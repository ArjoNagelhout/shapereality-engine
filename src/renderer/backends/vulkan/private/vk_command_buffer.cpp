//
// Created by Arjo Nagelhout on 19/11/2023.
//

#include "../vk_command_buffer.h"

namespace renderer
{
	VulkanCommandBuffer::VulkanCommandBuffer(renderer::CommandBuffer* commandBuffer) : CommandBufferImplementation(commandBuffer)
	{
	}

	VulkanCommandBuffer::~VulkanCommandBuffer() = default;
}