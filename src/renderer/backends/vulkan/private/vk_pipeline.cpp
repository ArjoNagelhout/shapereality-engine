//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../vk_pipeline.h"

namespace renderer
{
	VulkanPipeline::VulkanPipeline(Pipeline* pipeline) : PipelineImplementation(pipeline)
	{
	}

	VulkanPipeline::~VulkanPipeline() = default;
}