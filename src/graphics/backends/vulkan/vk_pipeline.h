//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_VK_PIPELINE_H
#define BORED_ENGINE_VK_PIPELINE_H

#include "../../pipeline.h"

namespace graphics
{
	class VulkanPipeline final : public PipelineImplementation
	{
	public:
		explicit VulkanPipeline(Pipeline* pipeline);
		~VulkanPipeline() override;
	};
}

#endif //BORED_ENGINE_VK_PIPELINE_H
