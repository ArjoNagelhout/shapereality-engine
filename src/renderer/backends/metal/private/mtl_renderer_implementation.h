//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_MTL_RENDERER_IMPLEMENTATION_H
#define BORED_ENGINE_MTL_RENDERER_IMPLEMENTATION_H

#include "../mtl_renderer.h"

#import <Metal/MTLDevice.h>

namespace renderer
{
	struct MetalRendererBackend::Implementation
	{
		id<MTLDevice> pDevice; // MTLDevice is a protocol, so we use id<MTLDevice> instead of MTLDevice*
	};
}

#endif //BORED_ENGINE_MTL_RENDERER_IMPLEMENTATION_H
