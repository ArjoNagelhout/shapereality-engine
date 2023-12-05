//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_PIPELINE_H
#define BORED_ENGINE_MTL_PIPELINE_H

#include "../../pipeline.h"

namespace graphics
{
	class MetalPipeline final : public PipelineImplementation
	{
	public:
		explicit MetalPipeline(Pipeline* pipeline);
		~MetalPipeline() override;
	};
}

#endif //BORED_ENGINE_MTL_PIPELINE_H
