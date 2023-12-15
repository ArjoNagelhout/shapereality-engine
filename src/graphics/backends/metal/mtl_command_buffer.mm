//
// Created by Arjo Nagelhout on 19/11/2023.
//

#include "mtl_command_buffer.h"

#include "mtl_render_pass.h"
#include "mtl_texture.h"
#include "mtl_render_pipeline_state.h"
#include "mtl_types.h"
#include "mtl_buffer.h"

#include <cassert>
#include <iostream>

namespace graphics
{
	MetalCommandBuffer::MetalCommandBuffer(id <MTLCommandBuffer> _pCommandBuffer) : pCommandBuffer(_pCommandBuffer)
	{
		// make sure the command buffer stays alive while the MetalCommandBuffer object exists
		// otherwise it could get destroyed by the autoreleasepool when we leave the scope
		// where the command buffer was received.
		[pCommandBuffer retain];
	}

	MetalCommandBuffer::~MetalCommandBuffer()
	{
		[pCommandBuffer release];
	}

	void MetalCommandBuffer::beginRenderPass(IRenderPass* renderPass)
	{
		assert(pRenderCommandEncoder == nullptr && "endRenderPass should have been called before calling this method");

		auto metalRenderPass = dynamic_cast<MetalRenderPass*>(renderPass);
		MTLRenderPassDescriptor* descriptor = metalRenderPass->pDescriptor;

		// create new render command encoder
		pRenderCommandEncoder = [pCommandBuffer renderCommandEncoderWithDescriptor:descriptor];
		[pRenderCommandEncoder retain];
	}

	void MetalCommandBuffer::endRenderPass(IRenderPass* renderPass)
	{
		assert(pRenderCommandEncoder != nullptr && "render command encoder should have been created before committing");

		[pRenderCommandEncoder endEncoding];
	}

	void MetalCommandBuffer::commit()
	{
		[pCommandBuffer commit];

		if (pRenderCommandEncoder != nullptr)
		{
			[pRenderCommandEncoder release];
		}
	}

	void MetalCommandBuffer::present(ITexture* texture)
	{
		auto metalTexture = dynamic_cast<MetalTexture*>(texture);
		[pCommandBuffer presentDrawable:metalTexture->getDrawable()];
	}

	void MetalCommandBuffer::setRenderPipelineState(IRenderPipelineState* renderPipelineState)
	{
		auto* metalPipelineState = dynamic_cast<MetalRenderPipelineState*>(renderPipelineState);
		[pRenderCommandEncoder setRenderPipelineState:metalPipelineState->get()];
	}

	void MetalCommandBuffer::setDepthStencilState(IDepthStencilState* depthStencilState)
	{
		auto* metalDepthStencilState = dynamic_cast<MetalDepthStencilState*>(depthStencilState);
		[pRenderCommandEncoder setDepthStencilState:metalDepthStencilState->get()];
	}

	void MetalCommandBuffer::setWindingOrder(WindingOrder windingOrder)
	{
		[pRenderCommandEncoder setFrontFacingWinding:convert(windingOrder)];
	}

	void MetalCommandBuffer::drawIndexedPrimitives(PrimitiveType primitiveType,
												   unsigned int indexCount,
												   IBuffer* _Nonnull indexBuffer,
												   unsigned int indexBufferOffset,
												   unsigned int instanceCount,
												   unsigned int baseVertex,
												   unsigned int baseInstance)
	{
		MTLPrimitiveType metalPrimitiveType = convert(primitiveType);
		auto* metalBuffer = dynamic_cast<MetalBuffer*>(indexBuffer);
		id<MTLBuffer> metalIndexBuffer = metalBuffer->getBuffer();
		MTLIndexType indexType = metalBuffer->getIndexType();
		[pRenderCommandEncoder drawIndexedPrimitives:metalPrimitiveType
										  indexCount:indexCount
										   indexType:indexType
										 indexBuffer:metalIndexBuffer
								   indexBufferOffset:indexBufferOffset
									   instanceCount:instanceCount
										  baseVertex:baseVertex
										baseInstance:baseInstance];
	}

	void MetalCommandBuffer::setVertexBuffer(IBuffer* _Nonnull buffer, unsigned int offset, unsigned int atIndex)
	{
		auto* metalBuffer = dynamic_cast<MetalBuffer*>(buffer);
		[pRenderCommandEncoder setVertexBuffer:metalBuffer->getBuffer() offset:offset atIndex:atIndex];
	}

	void MetalCommandBuffer::setCullMode(CullMode cullMode)
	{
		[pRenderCommandEncoder setCullMode:convert(cullMode)];
	}

	void MetalCommandBuffer::setViewport(Viewport viewport)
	{
		[pRenderCommandEncoder setViewport:convert(viewport)];
	}
}