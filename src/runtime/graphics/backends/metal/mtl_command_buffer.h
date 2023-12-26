//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef SHAPEREALITY_MTL_COMMAND_BUFFER_H
#define SHAPEREALITY_MTL_COMMAND_BUFFER_H

#include "graphics/command_buffer.h"
#include "graphics/buffer.h"

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

		void setDepthStencilState(IDepthStencilState* _Nonnull depthStencilState) override;

		void setWindingOrder(WindingOrder windingOrder) override;

		void setCullMode(CullMode cullMode) override;

		void setViewport(Viewport viewport) override;

		void drawIndexedPrimitives(PrimitiveType primitiveType,
								   unsigned int indexCount,
								   IBuffer* _Nonnull indexBuffer,
								   unsigned int indexBufferOffset,
								   unsigned int instanceCount,
								   unsigned int baseVertex,
								   unsigned int baseInstance) override;

		void setVertexStageBuffer(IBuffer* _Nonnull pBuffer, unsigned int offset, unsigned int atIndex) override;

		void setFragmentStageTexture(ITexture* _Nonnull pTexture, unsigned int atIndex) override;

	private:
		id<MTLCommandBuffer> _Nonnull pCommandBuffer;
		id<MTLRenderCommandEncoder> _Nullable pRenderCommandEncoder{nullptr}; // can be null if not initialized yet
	};
}

#endif //SHAPEREALITY_MTL_COMMAND_BUFFER_H
