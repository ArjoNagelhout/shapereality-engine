//
// Created by Arjo Nagelhout on 14/12/2023.
//

#include "mtl_buffer.h"
#include "mtl_device.h"
#include "mtl_types.h"

#include <graphics/command_queue.h>
#include <common/logger.h>

namespace graphics::metal
{
    MTLStorageMode storageMode(BufferUsage_ usage)
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

        if (((usage & BufferUsage_CPUWrite) == 0) && ((usage & BufferUsage_CPURead) == 0))
        {
            return MTLStorageModePrivate;
        }
        else
        {
            // todo: add checking for discrete GPU
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

    MetalBuffer::MetalBuffer(IDevice const* _Nonnull device_,
                             BufferDescriptor const& descriptor,
                             void* _Nonnull source,
                             bool take)
        : Buffer(descriptor), device(device_)
    {
        assert(descriptor.usage != BufferUsage_None && "buffer should at least have one use flag specified");
        assert(!take && "taking ownership of the provided source data is not yet implemented");

        id <MTLDevice> metalDevice = static_cast<MetalDevice const*>(device)->metalDevice();

        storageMode_ = storageMode(descriptor.usage);
        MTLResourceOptions options = resourceOptionsFromBufferUsage(descriptor_.usage);

        switch (storageMode_)
        {
            case MTLStorageModeShared:
            case MTLStorageModeManaged:
            {
                buffer = [metalDevice newBufferWithBytes:source length:descriptor_.size options:options];
                assert(buffer != nil && "failed to create buffer");
                break;
            }
            case MTLStorageModePrivate:
            {
                // if the storage mode is private, we have to use a staging buffer
                buffer = [metalDevice newBufferWithLength:descriptor_.size options:options];
                assert(buffer != nil && "failed to create buffer");

                set(source, descriptor_.size, 0, true); // uses staging buffer

                break;
            }
            default:
            {
                assert(false && "storage mode not supported");
            }
        }

        [buffer retain];
    }

    MetalBuffer::MetalBuffer(IDevice const* _Nonnull device_, BufferDescriptor const& descriptor)
        : Buffer(descriptor), device(device_)
    {
        assert(descriptor.usage != BufferUsage_None && "buffer should at least have one use flag specified");

        id <MTLDevice> metalDevice = dynamic_cast<MetalDevice const*>(device)->metalDevice();
        storageMode_ = storageMode(descriptor.usage);
        MTLResourceOptions options = resourceOptionsFromBufferUsage(descriptor_.usage);

        buffer = [metalDevice newBufferWithLength:descriptor_.size options:options];
        assert(buffer != nil && "failed to create buffer");
        [buffer retain];
    }

    MetalBuffer::~MetalBuffer()
    {
        [buffer release];
    }

    void MetalBuffer::set(void* _Nonnull source,
                          size_t size,
                          size_t offset,
                          bool synchronize_)
    {
        assert(offset + size <= descriptor_.size && "attempted to set memory outside range of buffer");

        switch (storageMode_)
        {
            case MTLStorageModeShared:
            case MTLStorageModeManaged:
            {
                // copy memory directly
                char* destination = static_cast<char*>([buffer contents]);
                memcpy(destination + offset, source, size);
                break;
            }
            case MTLStorageModePrivate:
            {
                // create staging buffer with bytes
                BufferDescriptor stagingBufferDescriptor{
                    .usage = static_cast<BufferUsage_>(BufferUsage_CPUWrite | BufferUsage_GPURead),
                    .size = descriptor_.size,
                    .stride = descriptor_.stride
                };
                std::unique_ptr<Buffer> stagingBuffer = device->createBuffer(stagingBufferDescriptor, source, false);

                ICommandQueue* queue = device->transferCommandQueue();
                std::unique_ptr<ICommandBuffer> commandBuffer = queue->getCommandBuffer();
                commandBuffer->copyBuffer(stagingBuffer.get(), 0, this, 0,
                                          descriptor_.size); // because we pass *this, we need to make sure the state of MetalBuffer is valid and can be used as an argument. This is the case, because we have already created buffer. Retaining is not an issue yet as it is not yet out of scope.
                commandBuffer->commit();

                break;
            }
            default:
            {
                assert(false && "unsupported storage mode for setting data");
            }
        }

        if (synchronize_)
        {
            synchronize(size, offset);
        }
    }

    void* _Nonnull MetalBuffer::take()
    {
        // todo
        assert(false && "take is not supported");
        switch (storageMode_)
        {
            case MTLStorageModeShared:
            case MTLStorageModeManaged:
            {
                break;
            }
            case MTLStorageModePrivate:
            {
                break;
            }
            default:
            {
                assert(false && "unsupported storage mode");
            }
        }

        //return nullptr;
    }

    void* _Nonnull MetalBuffer::get()
    {
        assert(((descriptor_.usage & BufferUsage_CPURead) != 0) &&
               "get() is not supported on buffer that does not have BufferUsage_CPURead set");

        switch (storageMode_)
        {
            case MTLStorageModeShared:
            case MTLStorageModeManaged:
            {
                return [buffer contents];
            }
            default:
            {
                assert(false && "get() is only support for buffers with storage mode Shared and Managed");
            }
        }
    }

    void MetalBuffer::synchronize(size_t size, size_t offset)
    {
        if (!requiresSynchronization())
        {
            return;
        }

        [buffer didModifyRange:NSRange{.location = offset, .length = size}];
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