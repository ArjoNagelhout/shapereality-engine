//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../vk_pipeline.h"

namespace renderer
{
	VulkanPipelineImplementation::VulkanPipelineImplementation(Pipeline* pipeline) : PipelineImplementation(pipeline)
	{
	}

	VulkanPipelineImplementation::~VulkanPipelineImplementation() = default;
}