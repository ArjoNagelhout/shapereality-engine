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

		[[nodiscard]] id<MTLRenderPipelineState> _Nonnull get() const;

	private:
		id<MTLRenderPipelineState> _Nonnull pRenderPipelineState;

	};

	class MetalDepthStencilState final : public IDepthStencilState
	{
	public:
		explicit MetalDepthStencilState(DepthStencilDescriptor const& descriptor, id<MTLDevice> _Nonnull pDevice);
		~MetalDepthStencilState() override;

		[[nodiscard]] id<MTLDepthStencilState> _Nonnull get() const;

	private:
		id<MTLDepthStencilState> _Nonnull pDepthStencilState;
	};
}

#endif //BORED_ENGINE_MTL_RENDER_PIPELINE_STATE_H
