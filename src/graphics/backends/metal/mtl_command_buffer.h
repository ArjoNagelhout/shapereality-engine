//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_MTL_COMMAND_BUFFER_H
#define BORED_ENGINE_MTL_COMMAND_BUFFER_H

#include "../../command_buffer.h"

#import <Metal/Metal.h>

namespace graphics
{
	class MetalCommandBuffer final : public ICommandBuffer
	{
	public:
		MetalCommandBuffer(CommandBufferDescription description, id<MTLDevice> _Nonnull device);
		~MetalCommandBuffer() override;

	private:

	};
}

#endif //BORED_ENGINE_MTL_COMMAND_BUFFER_H
