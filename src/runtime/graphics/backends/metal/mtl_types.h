//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef SHAPEREALITY_MTL_TYPES_H
#define SHAPEREALITY_MTL_TYPES_H

#include "graphics/graphics.h"

#import <Metal/Metal.h>

namespace graphics
{
	[[nodiscard]] MTLWinding convert(WindingOrder windingOrder);

	[[nodiscard]] MTLCullMode convert(CullMode cullMode);

	[[nodiscard]] MTLPrimitiveType convert(PrimitiveType primitiveType);

	[[nodiscard]] MTLCompareFunction convert(CompareFunction compareFunction);

	[[nodiscard]] MTLViewport convert(Viewport viewport);

	[[nodiscard]] NSRange convert(Range range);

	[[nodiscard]] MTLPixelFormat convert(PixelFormat pixelFormat);
}

#endif //SHAPEREALITY_MTL_TYPES_H
