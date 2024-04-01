//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef SHAPEREALITY_COMMAND_BUFFER_H
#define SHAPEREALITY_COMMAND_BUFFER_H

#include <cassert>
#include <graphics/types.h>

namespace graphics
{
    //@formatter:off
    class RenderPassDescriptor;
    class ITexture;
    class IRenderPipelineState;
    class IDepthStencilState;
    class Buffer;
    //@formatter:on

    class ICommandBuffer
    {
    public:
        virtual ~ICommandBuffer() = default;

        //
        virtual void beginRenderPass(RenderPassDescriptor const& descriptor) = 0;

        // ends the current render pass
        virtual void endRenderPass() = 0;

        //
        virtual void present(ITexture* texture) = 0;

        //
        virtual void enqueue() = 0;

        //
        virtual void commit() = 0;

        //
        virtual void setRenderPipelineState(IRenderPipelineState* renderPipelineState) = 0;

        //
        virtual void setDepthStencilState(IDepthStencilState* depthStencilState) = 0;

        //
        virtual void setWindingOrder(WindingOrder windingOrder) = 0;

        //
        virtual void setCullMode(CullMode cullMode) = 0;

        //
        virtual void setTriangleFillMode(TriangleFillMode triangleFillMode) = 0;

        //
        virtual void setViewport(Viewport viewport) = 0;

        //
        virtual void setScissorRect(ScissorRect scissorRect) = 0;

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
                                           Buffer* indexBuffer,
                                           unsigned int indexBufferOffset,
                                           unsigned int instanceCount,
                                           unsigned int baseVertex,
                                           unsigned int baseInstance) = 0;

        /**
         * sets the buffer for the vertex shader stage
         *
         * @param buffer
         * @param offset the offset from which the buffer will be read in the shader
         * @param atIndex at which index the shader can get the buffer's data
         */
        virtual void setVertexStageBuffer(Buffer* buffer, unsigned int offset, unsigned int atIndex) = 0;

        virtual void setVertexStageBufferOffset(unsigned int offset, unsigned int atIndex) = 0;

        // directly set single use data, smaller than 4KB, instead of having to create a buffer and call setVertexBuffer.
        virtual void setVertexStageBytes(void const* data, unsigned int length, unsigned int atIndex) = 0;

        //
        virtual void setFragmentStageTexture(ITexture* texture, unsigned int atIndex) = 0;
    };
}

#endif //SHAPEREALITY_COMMAND_BUFFER_H
