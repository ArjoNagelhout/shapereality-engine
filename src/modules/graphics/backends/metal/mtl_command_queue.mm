//
// Created by Arjo Nagelhout on 06/12/2023.
//

#include "mtl_command_queue.h"

#include "mtl_command_buffer.h"

namespace graphics
{
    MetalCommandQueue::MetalCommandQueue(id <MTLDevice> _Nonnull device, CommandQueueDescriptor const& descriptor)
    {
        commandQueue = [device newCommandQueue];
    }

    MetalCommandQueue::~MetalCommandQueue()
    {
        [commandQueue release];
    }

    std::unique_ptr<ICommandBuffer> MetalCommandQueue::getCommandBuffer() const
    {
        return std::make_unique<MetalCommandBuffer>(commandQueue.commandBuffer);
    }
}