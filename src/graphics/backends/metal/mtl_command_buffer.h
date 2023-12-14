//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_MTL_COMMAND_BUFFER_H
#define BORED_ENGINE_MTL_COMMAND_BUFFER_H

#include "../../command_buffer.h"
#import "../../buffer.h"

#import <Metal/Metal.h>

namespace graphics
{
	class MetalCommandBuffer final : public ICommandBuffer
	{
	public:
		explicit MetalCommandBuffer(id<MTLCommandBuffer> _Nonnull _pCommandBuffer);
		~MetalCommandBuffer() override;

		void beginRenderPass(IRenderPass* _Nonnull renderPass) override;

		void endRenderPass(IRenderPass* _Nonnull renderPass) override;

		void commit() override;

		void present(ITexture* _Nonnull texture) override;

		void setRenderPipelineState(IRenderPipelineState* _Nonnull renderPipelineState) override;

		void setWindingOrder(WindingOrder windingOrder) override;

		void drawIndexedPrimitives(PrimitiveType primitiveType,
								   unsigned int indexCount,
								   IBuffer* _Nonnull indexBuffer,
								   unsigned int indexBufferOffset,
								   unsigned int instanceCount,
								   unsigned int baseVertex,
								   unsigned int baseInstance) override;

		void setVertexBuffer(IBuffer* _Nonnull buffer, unsigned int offset, unsigned int atIndex) override;

		void setCullMode(CullMode cullMode) override;

	private:
		id<MTLCommandBuffer> _Nonnull pCommandBuffer;
		id<MTLRenderCommandEncoder> _Nullable pRenderCommandEncoder{nullptr}; // can be null if not initialized yet
	};
}

#endif //BORED_ENGINE_MTL_COMMAND_BUFFER_H
