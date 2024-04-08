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
    // based on how we intend to use the buffer, we use different
    // storage mechanisms and optimization flags for the graphics API
    enum BufferUsage_
    {
        BufferUsage_None = 0,
        BufferUsage_CPURead = 1 << 0, // if false, can improve CPU cache performance
        BufferUsage_CPUWrite = 1 << 1, // if true, we create a shared resource that exists on both CPU and GPU
        BufferUsage_GPURead = 1 << 2, // whether a shader on the GPU needs to read from the buffer
        BufferUsage_GPUWrite = 1 << 3, // whether a shader on the GPU needs to write to the buffer
        BufferUsage_All = (1 << 4) - 1
    };

    struct BufferDescriptor
    {
        BufferUsage_ usage = BufferUsage_None;
        size_t size = 0; // size of the buffer in bytes
        size_t stride = 0; // the size of each individual element in the buffer, useful for an index buffer for example
    };

    class Buffer
    {
    public:
        explicit Buffer(BufferDescriptor descriptor);

        virtual ~Buffer() = default;

        [[nodiscard]] BufferDescriptor const& descriptor() const;

        // only stride is exposed for editing, as the other descriptor's properties
        // would invalidate the buffer's state. usage and size are fixed for the
        // entire lifetime of the buffer.
        [[nodiscard]] size_t& stride();

        /**
         * set data from CPU
         *
         * if usage does not contain BufferUsage_CPUWrite, a staging buffer is used for a one-time data
         * upload to the GPU.
         *
         * @param source because void* does not support pointer arithmetic, the caller is responsible for offsetting source
         * @param size
         * @param offset destination offset in bytes
         * @param synchronize
         */
        virtual void set(void* source, size_t size, size_t offset, bool synchronize) = 0;

        // sets and synchronizes the whole buffer
        // assumes source is the same size in bytes as the buffer
        virtual void set(void* source, bool synchronize) = 0;

        // get data of buffer and take ownership of it, always supported, also when BufferUsage_CPURead is not set
        // however, in that case we create a temporary buffer for copying from GPU to CPU and the data is copied from
        // this staging buffer to CPU memory.
        // if BufferUsage_CPURead is set, it simply copies the data and provides a pointer to this copied data.
        [[nodiscard]] virtual void* take() = 0;

        // get data of buffer on CPU (only supported when usage contains BufferUsage_CPURead),
        // prefer using set if you're writing to the buffer, as it can automatically call `synchronize()` if needed
        [[nodiscard]] virtual void* get() = 0;

        // inform the GPU that the CPU has modified a section of the buffer
        virtual void synchronize(size_t size, size_t offset) = 0;

        // synchronize the whole buffer
        virtual void synchronize() = 0;

        // whether synchronization is needed after setting the buffer data
        [[nodiscard]] virtual bool requiresSynchronization() const = 0;

    protected:
        BufferDescriptor descriptor_;
    };
}

#endif //SHAPEREALITY_BUFFER_H
