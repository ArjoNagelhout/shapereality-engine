//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef SHAPEREALITY_MTL_RENDER_PIPELINE_STATE_H
#define SHAPEREALITY_MTL_RENDER_PIPELINE_STATE_H

#include "graphics/render_pipeline_state.h"

#import <Metal/Metal.h>

namespace graphics
{
    class MetalRenderPipelineState final : public IRenderPipelineState
    {
    public:
        explicit MetalRenderPipelineState(id <MTLDevice> _Nonnull pDevice, RenderPipelineDescriptor const& descriptor);

        ~MetalRenderPipelineState() override;

        [[nodiscard]] id <MTLRenderPipelineState> _Nonnull get() const;

    private:
        id <MTLRenderPipelineState> _Nonnull pRenderPipelineState;

    };

    class MetalDepthStencilState final : public IDepthStencilState
    {
    public:
        explicit MetalDepthStencilState(id <MTLDevice> _Nonnull pDevice, DepthStencilDescriptor const& descriptor);

        ~MetalDepthStencilState() override;

        [[nodiscard]] id <MTLDepthStencilState> _Nonnull get() const;

    private:
        id <MTLDepthStencilState> _Nonnull pDepthStencilState;
    };
}

#endif //SHAPEREALITY_MTL_RENDER_PIPELINE_STATE_H
