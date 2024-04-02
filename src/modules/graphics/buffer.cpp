//
// Created by Arjo Nagelhout on 27/03/2024.
//

#include "buffer.h"

namespace graphics
{
    Buffer::Buffer(BufferDescriptor descriptor) : descriptor_(descriptor) {}

    BufferDescriptor const& Buffer::descriptor() const
    {
        return descriptor_;
    }

    size_t& Buffer::stride()
    {
        return descriptor_.stride;
    }
}