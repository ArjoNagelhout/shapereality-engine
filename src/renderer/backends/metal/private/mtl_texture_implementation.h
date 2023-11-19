//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_MTL_TEXTURE_IMPLEMENTATION_H
#define BORED_ENGINE_MTL_TEXTURE_IMPLEMENTATION_H

#include "../mtl_texture.h"

#import <Metal/MTLTexture.h>

namespace renderer
{
	struct MetalTexture::Implementation
	{
		id<MTLTexture> pTexture;
	};
}

#endif //BORED_ENGINE_MTL_TEXTURE_IMPLEMENTATION_H
