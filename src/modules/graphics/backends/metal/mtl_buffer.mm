//
// Created by Arjo Nagelhout on 14/12/2023.
//

#include "mtl_buffer.h"

#include "mtl_types.h"

namespace graphics::metal
{
    MetalBuffer::MetalBuffer(id <MTLDevice> _Nonnull device, BufferDescriptor const& descriptor)
    {
        MTLResourceOptions options =
            static_cast<int>(MTLResourceUsageRead) | static_cast<int>(MTLResourceStorageModeShared);
        // options |=

        if (descriptor.data == nullptr)
        {
            // don't initialize with data
            buffer = [device newBufferWithLength:descriptor.size options:options];
        }
        else
        {
            // initialize with data
            buffer = [device newBufferWithBytes:descriptor.data length:descriptor.size options:options];
        }

        stride = descriptor.stride;

        [buffer retain];
    }

    MetalBuffer::~MetalBuffer()
    {
        [buffer release];
    }

    void* MetalBuffer::data()
    {
        return [buffer contents];
    }

    void MetalBuffer::update(Range range)
    {
        [buffer didModifyRange:convert(range)];
    }

    size_t MetalBuffer::size() const
    {
        return [buffer length];
    }

    id <MTLBuffer> _Nonnull MetalBuffer::get() const
    {
        return buffer;
    }

    MTLIndexType MetalBuffer::getIndexType() const
    {
        if (stride == sizeof(uint16_t)) // 16-bit
        {
            return MTLIndexTypeUInt16;
        }
        else if (stride == sizeof(uint32_t))
        {
            return MTLIndexTypeUInt32;
        }
        assert(false && "invalid stride set on buffer, should be either 16 bits or 32 bits");
    }
}