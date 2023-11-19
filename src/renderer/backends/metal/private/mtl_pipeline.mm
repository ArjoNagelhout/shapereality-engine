//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../mtl_pipeline.h"

namespace renderer
{
	MetalPipelineImplementation::MetalPipelineImplementation(Pipeline* pipeline) : PipelineImplementation(pipeline)
	{
	}

	MetalPipelineImplementation::~MetalPipelineImplementation() = default;
}