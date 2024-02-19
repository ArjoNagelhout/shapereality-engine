//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef SHAPEREALITY_BUFFER_H
#define SHAPEREALITY_BUFFER_H

#include <cassert>
#include <cstddef>

#include <graphics/graphics.h>

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

        StorageMode storageMode;

        // can be null
        void const* data{nullptr};

        // size of the buffer in bytes
        unsigned int length;

        // the size of each individual element in the buffer
        // useful for an index buffer for example
        size_t stride;
    };

    class IBuffer
    {
    public:
        virtual ~IBuffer() = default;

        // get the system address of the buffer's storage allocation.
        // note: if the buffer has StorageMode::Private, this returns nullptr!
        [[nodiscard]] virtual void* getContents() = 0;

        // inform the GPU that the CPU has modified a section of the buffer
        virtual void didModifyRange(Range range) = 0;

        //
        [[nodiscard]] virtual unsigned int getLength() const = 0;

        size_t stride{};
    };
}

#endif //SHAPEREALITY_BUFFER_H
