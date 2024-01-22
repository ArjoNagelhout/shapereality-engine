//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef SHAPEREALITY_MTL_TYPES_H
#define SHAPEREALITY_MTL_TYPES_H

#include "graphics/graphics.h"
#include "graphics/render_pass.h"
#include "graphics/texture.h"

#import <Metal/Metal.h>

namespace graphics
{
    // even though not all of these functions are used across the codebase,
    // it makes it easy to see if we have already defined a conversion function,
    // because otherwise it might be duplicated inside different translation units

    [[nodiscard]] MTLWinding convert(WindingOrder windingOrder);

    [[nodiscard]] MTLCullMode convert(CullMode cullMode);

    [[nodiscard]] MTLTriangleFillMode convert(TriangleFillMode triangleFillMode);

    [[nodiscard]] MTLPrimitiveType convert(PrimitiveType primitiveType);

    [[nodiscard]] MTLCompareFunction convert(CompareFunction compareFunction);

    [[nodiscard]] MTLViewport convert(Viewport viewport);

    [[nodiscard]] NSRange convert(Range range);

    [[nodiscard]] MTLPixelFormat convert(PixelFormat pixelFormat);

    [[nodiscard]] PixelFormat convert(MTLPixelFormat pixelFormat);

    [[nodiscard]] MTLScissorRect convert(ScissorRect scissorRect);

    // StoreAction

    // warning: lossy, StoreAction does not cover all cases
    [[nodiscard]] StoreAction convert(MTLStoreAction action);

    [[nodiscard]] MTLStoreAction convert(StoreAction action);

    // LoadAction

    [[nodiscard]] LoadAction convert(MTLLoadAction action);

    [[nodiscard]] MTLLoadAction convert(LoadAction action);

    // MultisampleDepthResolveFilter

    [[nodiscard]] MTLMultisampleDepthResolveFilter convert(MultisampleDepthResolveFilter filter);

    [[nodiscard]] MultisampleDepthResolveFilter convert(MTLMultisampleDepthResolveFilter filter);

    // Color

    [[nodiscard]] Color convert(MTLClearColor color);

    [[nodiscard]] MTLClearColor convert(Color const& color);

    // Texture

    [[nodiscard]] MTLTextureUsage convert(TextureUsage_ usage);

    // named differently because both types are ints and thus have the same function signature, might be a good idea
    // to change all convert() function names to reflect this naming convention.
    [[nodiscard]] TextureUsage_ convertFromMetal(MTLTextureUsage usage);

    [[nodiscard]] MTLTextureType convert(TextureType textureType);

    [[nodiscard]] TextureType convert(MTLTextureType textureType);
}

#endif //SHAPEREALITY_MTL_TYPES_H
