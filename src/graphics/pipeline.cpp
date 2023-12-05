//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "pipeline.h"

#include "backends/metal/mtl_pipeline.h"
#include "backends/vulkan/vk_pipeline.h"

namespace graphics
{
	PipelineImplementation::PipelineImplementation(Pipeline* pipeline) : pPipeline(pipeline)
	{
	}

	PipelineImplementation::~PipelineImplementation() = default;

	Pipeline::Pipeline() = default;

	Pipeline::~Pipeline() = default;

	void Pipeline::onGraphicsBackendChanged(GraphicsBackendType const& rendererBackendType)
	{
		switch (rendererBackendType)
		{
			case GraphicsBackendType::Metal:
				pImplementation = std::make_unique<MetalPipeline>(this);
				break;
			case GraphicsBackendType::Vulkan:
				pImplementation = std::make_unique<VulkanPipeline>(this);
				break;
			default:
				pImplementation.reset();
		}
	}
}