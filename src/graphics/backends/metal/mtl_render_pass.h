//
// Created by Arjo Nagelhout on 09/12/2023.
//

#ifndef BORED_ENGINE_MTL_RENDER_PASS_H
#define BORED_ENGINE_MTL_RENDER_PASS_H

#include "../../render_pass.h"

#import <Metal/Metal.h>

namespace graphics
{
	class MetalRenderPass final : public IRenderPass
	{
	public:
		explicit MetalRenderPass();
		~MetalRenderPass() override;

	private:

	};
}

#endif //BORED_ENGINE_MTL_RENDER_PASS_H
