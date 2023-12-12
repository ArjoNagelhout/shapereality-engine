//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef BORED_ENGINE_MTL_RENDER_PIPELINE_STATE_H
#define BORED_ENGINE_MTL_RENDER_PIPELINE_STATE_H

#include "../../render_pipeline_state.h"

#import <Metal/Metal.h>

namespace graphics
{
	class MetalRenderPipelineState final : public IRenderPipelineState
	{
	public:
		explicit MetalRenderPipelineState(RenderPipelineDescriptor const& descriptor, id<MTLDevice> _Nonnull pDevice);
		~MetalRenderPipelineState() override;

	private:

	};
}

#endif //BORED_ENGINE_MTL_RENDER_PIPELINE_STATE_H
