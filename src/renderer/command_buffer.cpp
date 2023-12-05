//
// Created by Arjo Nagelhout on 15/11/2023.
//

#include "command_buffer.h"

#include "backends/metal/mtl_command_buffer.h"
#include "backends/vulkan/vk_command_buffer.h"

namespace renderer
{
	CommandBufferImplementation::CommandBufferImplementation(CommandBuffer* commandBuffer) : pCommandBuffer(commandBuffer)
	{
	}

	CommandBufferImplementation::~CommandBufferImplementation() = default;

	CommandBuffer::CommandBuffer() = default;

	CommandBuffer::~CommandBuffer() = default;

	void CommandBuffer::onGraphicsBackendChanged(GraphicsBackendType const& rendererBackendType)
	{
		switch (rendererBackendType)
		{
			case GraphicsBackendType::Metal:
				pImplementation = std::make_unique<MetalCommandBuffer>(this);
				break;
			case GraphicsBackendType::Vulkan:
				pImplementation = std::make_unique<VulkanCommandBuffer>(this);
				break;
			default:
				pImplementation.reset();
		}
	}

	void CommandBuffer::blit()
	{
	}
}