//
// Created by Arjo Nagelhout on 06/12/2023.
//

#ifndef SHAPEREALITY_MTL_COMMAND_QUEUE_H
#define SHAPEREALITY_MTL_COMMAND_QUEUE_H

#include "graphics/command_queue.h"

#import <Metal/Metal.h>

namespace graphics::metal
{
    class MetalCommandQueue final : public ICommandQueue
    {
    public:
        explicit MetalCommandQueue(id <MTLDevice> _Nonnull device, CommandQueueDescriptor const& descriptor);

        ~MetalCommandQueue() override;

        [[nodiscard]] std::unique_ptr<ICommandBuffer> getCommandBuffer() const override;

    private:
        id <MTLCommandQueue> _Nonnull commandQueue;
    };
}

#endif //SHAPEREALITY_MTL_COMMAND_QUEUE_H
