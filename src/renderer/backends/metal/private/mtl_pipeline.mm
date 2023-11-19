//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../mtl_pipeline.h"

namespace renderer
{
	MetalPipeline::MetalPipeline(Pipeline* pipeline) : PipelineImplementation(pipeline)
	{
	}

	MetalPipeline::~MetalPipeline() = default;
}