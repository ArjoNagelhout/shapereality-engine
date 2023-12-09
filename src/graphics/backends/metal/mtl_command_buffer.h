//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_MTL_COMMAND_BUFFER_H
#define BORED_ENGINE_MTL_COMMAND_BUFFER_H

#include "../../command_buffer.h"

#import <Metal/Metal.h>

namespace graphics
{
	class MetalCommandBuffer final : public ICommandBuffer
	{
	public:
		explicit MetalCommandBuffer(id<MTLCommandBuffer> _pCommandBuffer);
		~MetalCommandBuffer() override;

		void beginRenderPass() override;
		void endRenderPass() override;

	private:
		id<MTLCommandBuffer> pCommandBuffer;
		id<MTLRenderCommandEncoder> _Nullable pRenderCommandEncoder{nullptr}; // can be null if not initialized yet
	};
}

#endif //BORED_ENGINE_MTL_COMMAND_BUFFER_H
