//
// Created by Arjo Nagelhout on 09/12/2023.
//

#ifndef BORED_ENGINE_MTL_RENDER_PASS_H
#define BORED_ENGINE_MTL_RENDER_PASS_H

#include "graphics/render_pass.h"

#import <Metal/Metal.h>

namespace graphics
{
	class MetalRenderPass final : public IRenderPass
	{
	public:
		explicit MetalRenderPass(RenderPassDescriptor descriptor);
		~MetalRenderPass() override;

		explicit MetalRenderPass(MTLRenderPassDescriptor* _Nonnull descriptor);

		MTLRenderPassDescriptor* _Nonnull pDescriptor;
	};
}

#endif //BORED_ENGINE_MTL_RENDER_PASS_H
