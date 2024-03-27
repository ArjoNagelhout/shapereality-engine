//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef SHAPEREALITY_BUFFER_H
#define SHAPEREALITY_BUFFER_H

#include <cassert>
#include <cstddef>

#include <graphics/types.h>

namespace graphics
{
    struct BufferDescriptor
    {
        enum class StorageMode
        {
            Private, // only on GPU
            Shared, // on GPU and CPU
            Managed // on GPU and CPU, but memory is backed by private storage, so can still be optimised
        };

        StorageMode storageMode = StorageMode::Shared;
        void const* data = nullptr; // can be null
        size_t size = 0; // size of the buffer in bytes
        size_t stride = 0; // the size of each individual element in the buffer, useful for an index buffer for example
    };

    class IBuffer
    {
    public:
        virtual ~IBuffer() = default;

        // get the system address of the buffer's storage allocation.
        // note: if the buffer has StorageMode::Private, this returns nullptr!
        [[nodiscard]] virtual void* data() = 0;

        // inform the GPU that the CPU has modified a section of the buffer
        virtual void update(Range range) = 0;

        // size of the buffer in bytes
        [[nodiscard]] virtual size_t size() const = 0;

        size_t stride = 0;
    };
}

#endif //SHAPEREALITY_BUFFER_H
