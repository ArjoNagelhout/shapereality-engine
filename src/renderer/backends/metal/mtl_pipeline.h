//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_PIPELINE_H
#define BORED_ENGINE_MTL_PIPELINE_H

#include "../../pipeline.h"

namespace renderer
{
	class MetalPipelineImplementation : public PipelineImplementation
	{
	public:
		explicit MetalPipelineImplementation(Pipeline* pipeline);
		~MetalPipelineImplementation() override;
	};
}

#endif //BORED_ENGINE_MTL_PIPELINE_H
