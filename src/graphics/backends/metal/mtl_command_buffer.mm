//
// Created by Arjo Nagelhout on 19/11/2023.
//

#include "mtl_command_buffer.h"

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

		std::cout << "created metal command buffer" << std::endl;
	}

	MetalCommandBuffer::~MetalCommandBuffer()
	{
		[pCommandBuffer release];

		std::cout << "destroyed metal command buffer" << std::endl;
	}

	void MetalCommandBuffer::beginRenderPass()
	{
		assert(pRenderCommandEncoder == nullptr && "endRenderPass should have been called before calling this method");

		MTLRenderPassDescriptor* descriptor = [[MTLRenderPassDescriptor alloc] init];

		// create new render command encoder
		pRenderCommandEncoder = [pCommandBuffer renderCommandEncoderWithDescriptor:descriptor];

		//id<MTLRenderCommandEncoder> encoder;

		[descriptor release];
	}

	void MetalCommandBuffer::endRenderPass()
	{
		assert(pRenderCommandEncoder != nullptr && "render command encoder should have been created before committing");
		[pRenderCommandEncoder release];
	}
}