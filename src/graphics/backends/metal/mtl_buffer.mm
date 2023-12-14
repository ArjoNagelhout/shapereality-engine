//
// Created by Arjo Nagelhout on 14/12/2023.
//

#include "mtl_buffer.h"

namespace graphics
{
	MetalBuffer::MetalBuffer(BufferDescriptor const& descriptor, id<MTLDevice> _Nonnull pDevice)
	{
		MTLResourceOptions options = 0;
		// options |=

		if (descriptor.data == nullptr)
		{
			// don't initialize with data
			pBuffer = [pDevice newBufferWithLength:descriptor.length options:options];
		}
		else
		{
			// initialize with data
			pBuffer = [pDevice newBufferWithBytes:descriptor.data length: descriptor.length options:options];
		}

		[pBuffer retain];
	}

	MetalBuffer::~MetalBuffer()
	{
		[pBuffer release];
	}

	id <MTLBuffer> _Nonnull MetalBuffer::getBuffer() const
	{
		return pBuffer;
	}
}