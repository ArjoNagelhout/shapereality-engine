//
// Created by Arjo Nagelhout on 14/12/2023.
//

#include "mtl_buffer.h"

#include "mtl_types.h"

#include <common/application_info.h>

namespace graphics::metal
{
    MTLStorageMode storageMode(BufferUsage_ usage)
    {
        if (((usage & BufferUsage_CPUWrite) == 0) && ((usage & BufferUsage_CPURead) == 0))
        {
            return MTLStorageModePrivate;
        }
        else
        {
            // buffer should be accessible from the CPU
            // depending on whether we are on macOS or iOS, visionOS or tvOS, we use
            // managed or shared
            // https://supertrouper.gitbooks.io/metal-programming-guide/content/whats-new-in-ios-9-and-os-x-1011whats-new-in-ios-9-and-os-x-1011md.html#private
            // https://developer.apple.com/documentation/metal/resource_fundamentals/choosing_a_resource_storage_mode_for_intel_and_amd_gpus?language=objc
            https://developer.apple.com/documentation/metal/resource_fundamentals/choosing_a_resource_storage_mode_for_apple_gpus?language=objc

            // shared resources are only available on integrated graphics (Apple Silicon, Intel-based Mac computers)
            // so Managed is better if there is a discrete GPU, otherwise use Shared
            // https://developer.apple.com/metal/Metal-Feature-Set-Tables.pdf

            bool discreteGPU = false;
            if (discreteGPU)
            {
                return MTLStorageModeManaged;
            }
            else
            {
                return MTLStorageModeShared;
            }
        }
    }

    MTLResourceOptions resourceOptionsFromBufferUsage(BufferUsage_ usage)
    {
        MTLResourceOptions options = 0;
        if ((usage & BufferUsage_CPURead) == 0)
        {
            // if we don't need to read from the CPU, we can set MTLCPUCacheModeWriteCombined
            // warning:
            // - reading from write combined memory is god-awfully slow, so we should disallow reading at all.
            // - writes should be large continuous ranges, without holes, as that would create multiple writes (slow)
            // source: https://fgiesen.wordpress.com/2013/01/29/write-combining-is-not-your-friend/
            options |= MTLResourceCPUCacheModeWriteCombined;
        }
        else
        {
            options |= MTLResourceCPUCacheModeDefaultCache; // read and write actions are guaranteed to always be sequential
        }
        MTLStorageMode storageMode_ = storageMode(usage);
        options |= storageMode_ << MTLResourceStorageModeShift;
        return options;
    }

    MetalBuffer::MetalBuffer(id <MTLDevice> _Nonnull device,
                             BufferDescriptor const& descriptor,
                             void* _Nonnull source,
                             bool take)
        : Buffer(descriptor)
    {
        assert(!take && "taking ownership of the provided source data is not yet implemented");

        storageMode_ = storageMode(descriptor.usage);
        MTLResourceOptions options = resourceOptionsFromBufferUsage(descriptor_.usage);

        buffer = [device newBufferWithBytes:source length:descriptor_.size options:options];
        assert(buffer != nil && "failed to create buffer");
        [buffer retain];
    }

    MetalBuffer::MetalBuffer(id <MTLDevice> _Nonnull device, BufferDescriptor const& descriptor) : Buffer(descriptor)
    {
        storageMode_ = storageMode(descriptor.usage);
        MTLResourceOptions options = resourceOptionsFromBufferUsage(descriptor_.usage);

        buffer = [device newBufferWithLength:descriptor_.size options:options];
        assert(buffer != nil && "failed to create buffer");
        [buffer retain];
    }

    MetalBuffer::~MetalBuffer()
    {
        [buffer release];
    }

    void MetalBuffer::set(void* _Nonnull source,
                          size_t size,
                          size_t sourceOffset,
                          size_t destinationOffset,
                          bool synchronize_)
    {


        if (synchronize_)
        {
            synchronize(size, destinationOffset);
        }
    }

    void* _Nonnull MetalBuffer::take()
    {
        return nullptr;
    }

    void* _Nonnull MetalBuffer::get()
    {
        assert(((descriptor_.usage & BufferUsage_CPURead) != 0) && "get() is not supported on buffer that does not have BufferUsage_CPURead set");

        return nullptr;
    }

    void MetalBuffer::synchronize(size_t size, size_t offset)
    {
        if (!requiresSynchronization())
        {
            return;
        }
    }

    id <MTLBuffer> _Nonnull MetalBuffer::metalBuffer() const
    {
        return buffer;
    }

    bool MetalBuffer::requiresSynchronization() const
    {
        return storageMode_ == MTLStorageModeManaged;
    }

    MTLIndexType indexType(size_t size)
    {
        if (size == sizeof(uint16_t)) // 16-bit
        {
            return MTLIndexTypeUInt16;
        }
        else if (size == sizeof(uint32_t))
        {
            return MTLIndexTypeUInt32;
        }
        assert(false && "invalid size for index type, expected 16 or 32 bits");
    }
}