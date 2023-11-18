//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_VK_PIPELINE_H
#define BORED_ENGINE_VK_PIPELINE_H

#include "../../pipeline.h"

namespace renderer
{
	class VulkanPipelineImplementation : public PipelineImplementation
	{
	public:
		explicit VulkanPipelineImplementation(Pipeline* pipeline);
		~VulkanPipelineImplementation() override;
	};
}

#endif //BORED_ENGINE_VK_PIPELINE_H
