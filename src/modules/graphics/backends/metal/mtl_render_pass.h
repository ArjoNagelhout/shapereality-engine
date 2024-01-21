//
// Created by Arjo Nagelhout on 09/12/2023.
//

#ifndef SHAPEREALITY_MTL_RENDER_PASS_H
#define SHAPEREALITY_MTL_RENDER_PASS_H

#include "graphics/render_pass.h"

#import <Metal/Metal.h>

namespace graphics
{
    // convert a MTLRenderPassDescriptor to its platform-agnostic counterpart
    // this is needed because for example when we get the render pass descriptor from
    // a view, we want to get information about the textures (e.g. for creating an
    // ImGui implementation).
    [[nodiscard]] std::unique_ptr<RenderPassDescriptor> createRenderPassDescriptor(MTLRenderPassDescriptor* _Nonnull descriptor);

    class MetalRenderPass final : public IRenderPass
    {
    public:
        explicit MetalRenderPass(id <MTLDevice> _Nonnull pDevice, RenderPassDescriptor const& descriptor);

        ~MetalRenderPass() override;

        explicit MetalRenderPass(MTLRenderPassDescriptor* _Nonnull descriptor);

        MTLRenderPassDescriptor* _Nonnull pDescriptor;
    };
}

#endif //SHAPEREALITY_MTL_RENDER_PASS_H
