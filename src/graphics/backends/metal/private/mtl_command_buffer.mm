//
// Created by Arjo Nagelhout on 19/11/2023.
//

#include "../mtl_command_buffer.h"

namespace graphics
{
	MetalCommandBuffer::MetalCommandBuffer(graphics::CommandBuffer* commandBuffer) : CommandBufferImplementation(commandBuffer)
	{
	}

	MetalCommandBuffer::~MetalCommandBuffer() = default;
}