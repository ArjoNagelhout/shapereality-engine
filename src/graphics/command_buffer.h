//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_COMMAND_BUFFER_H
#define BORED_ENGINE_COMMAND_BUFFER_H

#include <cassert>
#include "graphics.h"

namespace graphics
{
	class IRenderPass;
	class ITexture;
	class IRenderPipelineState;
	class IDepthStencilState;
	class IBuffer;

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

		//
		virtual void setRenderPipelineState(IRenderPipelineState* renderPipelineState)
		{
			assert(false && "interface class method should not be called");
		}

		//
		virtual void setDepthStencilState(IDepthStencilState* depthStencilState)
		{
			assert(false && "interface class method should not be called");
		}

		//
		virtual void setWindingOrder(WindingOrder windingOrder)
		{
			assert(false && "interface class method should not be called");
		}

		//
		virtual void setViewport(Viewport viewport)
		{
			assert(false && "interface class method should not be called");
		}

		/**
		 * renders multiple instances of a geometric primitive with indexed vertices,
		 * starting with a custom vertex and instance_id
		 *
		 * @param primitiveType
		 * @param indexCount number of vertices read from indexBuffer for each instance
		 * @param indexBuffer
		 * @param indexBufferOffset
		 * @param instanceCount
		 * @param baseVertex
		 * @param baseInstance
		 */
		virtual void drawIndexedPrimitives(PrimitiveType primitiveType,
										   unsigned int indexCount,
										   IBuffer* indexBuffer,
										   unsigned int indexBufferOffset,
										   unsigned int instanceCount,
										   unsigned int baseVertex,
										   unsigned int baseInstance)
		{
			assert(false && "interface class method should not be called");
		}

		/**
		 * sets the buffer for the vertex shader stage
		 *
		 * @param buffer
		 * @param offset the offset from which the buffer will be read in the shader
		 * @param atIndex at which index the shader can get the buffer's data
		 */
		virtual void setBufferForVertexStage(IBuffer* buffer, unsigned int offset, unsigned int atIndex)
		{
			assert(false && "interface class method should not be called");
		}

		//
		virtual void setCullMode(CullMode cullMode)
		{
			assert(false && "interface class method should not be called");
		}
	};
}

#endif //BORED_ENGINE_COMMAND_BUFFER_H
