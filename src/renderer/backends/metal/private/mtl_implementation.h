//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_MTL_IMPLEMENTATION_H
#define BORED_ENGINE_MTL_IMPLEMENTATION_H

#include "../mtl_graphics.h"
#include "../mtl_texture.h"

#import <Metal/MTLDevice.h>

namespace renderer
{
	// implementations that we need to hide from the headers, as otherwise we would need to
	// compile the entire engine with Objective-C++ or Objective-C, which is undesirable.

	struct MetalGraphicsBackend::Implementation
	{
		id<MTLDevice> pDevice; // MTLDevice is a protocol, so we use id<MTLDevice> instead of MTLDevice*
	};

	struct MetalTexture::Implementation
	{
		id<MTLTexture> pTexture;
	};
}

#endif //BORED_ENGINE_MTL_IMPLEMENTATION_H
