//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_COMMAND_BUFFER_H
#define BORED_ENGINE_COMMAND_BUFFER_H

#include <cassert>

namespace graphics
{
	class IRenderPass;
	class ITexture;

	class ICommandBuffer
	{
	public:
		virtual ~ICommandBuffer() = default;

		//
		virtual void beginRenderPass(IRenderPass* renderPass)
		{
			assert(false && "interface class method should not be called");
		}

		//
		virtual void endRenderPass(IRenderPass* renderPass)
		{
			assert(false && "interface class method should not be called");
		}

		//
		virtual void present(ITexture* texture)
		{
			assert(false && "interface class method should not be called");
		}

		//
		virtual void enqueue()
		{
			assert(false && "interface class method should not be called");
		}

		//
		virtual void commit()
		{
			assert(false && "interface class method should not be called");
		}
	};
}

#endif //BORED_ENGINE_COMMAND_BUFFER_H
