//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "pipeline.h"

#include "backends/metal/mtl_pipeline.h"
#include "backends/vulkan/vk_pipeline.h"

namespace renderer
{
	PipelineImplementation::PipelineImplementation(Pipeline* pipeline) : pPipeline(pipeline)
	{
	}

	PipelineImplementation::~PipelineImplementation() = default;

	Pipeline::Pipeline() = default;

	Pipeline::~Pipeline() = default;

	void Pipeline::onRendererBackendChanged(RendererBackendType const& rendererBackendType)
	{
		switch (rendererBackendType)
		{
			case RendererBackendType::Metal:
				pImplementation = std::make_unique<MetalPipelineImplementation>(this);
				break;
			case RendererBackendType::Vulkan:
				pImplementation = std::make_unique<VulkanPipelineImplementation>(this);
				break;
			default:
				pImplementation.reset();
		}
	}
}