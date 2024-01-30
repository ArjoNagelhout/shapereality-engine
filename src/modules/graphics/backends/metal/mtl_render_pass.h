//
// Created by Arjo Nagelhout on 09/12/2023.
//

#ifndef SHAPEREALITY_MTL_RENDER_PASS_H
#define SHAPEREALITY_MTL_RENDER_PASS_H

#include "graphics/render_pass.h"

#import <Metal/Metal.h>

namespace graphics
{
    // converts MTLRenderPassDescriptor to platform-agnostic render pass descriptor
    [[nodiscard]] std::unique_ptr<RenderPassDescriptor>
    createRenderPassDescriptor(MTLRenderPassDescriptor* _Nonnull descriptor);

    // converts platform-agnostic render pass descriptor to MTLRenderPassDescriptor
    // ownership is transferred to the caller.
    [[nodiscard]] MTLRenderPassDescriptor* _Nonnull
    createRenderPassDescriptor(RenderPassDescriptor const& descriptor);
}

#endif //SHAPEREALITY_MTL_RENDER_PASS_H
