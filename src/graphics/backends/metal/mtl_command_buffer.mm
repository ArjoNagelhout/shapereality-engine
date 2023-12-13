//
// Created by Arjo Nagelhout on 19/11/2023.
//

#include "mtl_command_buffer.h"

#include "mtl_render_pass.h"
#include "mtl_texture.h"
#include "mtl_render_pipeline_state.h"

#include <cassert>
#include <iostream>

namespace graphics
{
	MetalCommandBuffer::MetalCommandBuffer(id<MTLCommandBuffer> _pCommandBuffer) : pCommandBuffer(_pCommandBuffer)
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

		[pCommandBuffer presentDrawable: metalTexture->getDrawable()];
	}

	void MetalCommandBuffer::setRenderPipelineState(IRenderPipelineState* renderPipelineState)
	{
		auto* metalPipelineState = dynamic_cast<MetalRenderPipelineState*>(renderPipelineState);

	}
}