//
// Created by Arjo Nagelhout on 14/12/2023.
//

#include "mtl_buffer.h"

#include "mtl_types.h"

namespace graphics
{
    MetalBuffer::MetalBuffer(id<MTLDevice> _Nonnull pDevice, BufferDescriptor const& descriptor)
    {
        MTLResourceOptions options = MTLResourceUsageRead | MTLResourceStorageModeShared;
        // options |=

        if (descriptor.data == nullptr)
        {
            // don't initialize with data
            pBuffer = [pDevice newBufferWithLength:descriptor.length options:options];
        }
        else
        {
            // initialize with data
            pBuffer = [pDevice newBufferWithBytes:descriptor.data length:descriptor.length options:options];
        }

        // set index type (move out to IBuffer if this is common to do across rendering pipelines)
        if (descriptor.type == BufferDescriptor::Type::Index)
        {
            if (descriptor.stride == sizeof(uint16_t)) // 16-bit
            {
                indexType = MTLIndexTypeUInt16;
            }
            else if (descriptor.stride == sizeof(uint32_t))
            {
                indexType = MTLIndexTypeUInt32;
            }
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

    MTLIndexType MetalBuffer::getIndexType() const
    {
        return indexType;
    }

    void* MetalBuffer::getContents()
    {
        return [pBuffer contents];
    }

    void MetalBuffer::didModifyRange(Range range)
    {
        [pBuffer didModifyRange:convert(range)];
    }
}