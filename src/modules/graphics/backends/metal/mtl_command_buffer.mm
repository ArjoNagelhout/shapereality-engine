//
// Created by Arjo Nagelhout on 19/11/2023.
//

#include "mtl_command_buffer.h"

#include "mtl_render_pass.h"
#include "mtl_texture.h"
#include "mtl_render_pipeline_state.h"
#include "mtl_types.h"
#include "mtl_buffer.h"

#include <cassert>
#include <iostream>

namespace graphics::metal
{
    MetalCommandBuffer::MetalCommandBuffer(id <MTLCommandBuffer> _commandBuffer) : commandBuffer(_commandBuffer)
    {
        // make sure the command buffer stays alive while the MetalCommandBuffer object exists
        // otherwise it could get destroyed by the autoreleasepool when we leave the scope
        // where the command buffer was received.
        [commandBuffer retain];
    }

    MetalCommandBuffer::~MetalCommandBuffer()
    {
        [commandBuffer release];
    }

    void MetalCommandBuffer::beginRenderPass(RenderPassDescriptor const& descriptor)
    {
        assert(renderCommandEncoder == nullptr && "endRenderPass should have been called before calling this method");

        MTLRenderPassDescriptor* metalDescriptor = createRenderPassDescriptor(descriptor);

        // create new render command encoder
        renderCommandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:metalDescriptor];
        [renderCommandEncoder retain];
    }

    void MetalCommandBuffer::endRenderPass()
    {
        assert(renderCommandEncoder != nullptr && "render command encoder should have been created before committing");

        [renderCommandEncoder endEncoding];
    }

    void MetalCommandBuffer::present(ITexture* texture)
    {
        auto metalTexture = static_cast<MetalTexture*>(texture);
        [commandBuffer presentDrawable:metalTexture->getDrawable()];
    }

    void MetalCommandBuffer::enqueue()
    {

    }

    void MetalCommandBuffer::commit()
    {
        [commandBuffer commit];

        if (renderCommandEncoder != nullptr)
        {
            [renderCommandEncoder release];
        }
    }

    void MetalCommandBuffer::setRenderPipelineState(IRenderPipelineState* renderPipelineState)
    {
        auto* metalPipelineState = static_cast<MetalRenderPipelineState*>(renderPipelineState);
        [renderCommandEncoder setRenderPipelineState:metalPipelineState->get()];
    }

    void MetalCommandBuffer::setDepthStencilState(IDepthStencilState* depthStencilState)
    {
        auto* metalDepthStencilState = static_cast<MetalDepthStencilState*>(depthStencilState);
        [renderCommandEncoder setDepthStencilState:metalDepthStencilState->get()];
    }

    void MetalCommandBuffer::setWindingOrder(WindingOrder windingOrder)
    {
        [renderCommandEncoder setFrontFacingWinding:convert(windingOrder)];
    }

    void MetalCommandBuffer::setCullMode(CullMode cullMode)
    {
        [renderCommandEncoder setCullMode:convert(cullMode)];
    }

    void MetalCommandBuffer::setTriangleFillMode(TriangleFillMode triangleFillMode)
    {
        [renderCommandEncoder setTriangleFillMode:convert(triangleFillMode)];
    }

    void MetalCommandBuffer::setViewport(Viewport viewport)
    {
        [renderCommandEncoder setViewport:convert(viewport)];
    }

    void MetalCommandBuffer::setScissorRect(ScissorRect scissorRect)
    {
        [renderCommandEncoder setScissorRect:convert(scissorRect)];
    }

    void MetalCommandBuffer::drawIndexedPrimitives(PrimitiveType primitiveType,
                                                   unsigned int indexCount,
                                                   Buffer* _Nonnull indexBuffer,
                                                   unsigned int indexBufferOffset,
                                                   unsigned int instanceCount,
                                                   unsigned int baseVertex,
                                                   unsigned int baseInstance)
    {
        MTLPrimitiveType metalPrimitiveType = convert(primitiveType);
        auto* metalBuffer = static_cast<MetalBuffer*>(indexBuffer);
        id <MTLBuffer> metalIndexBuffer = metalBuffer->metalBuffer();
        MTLIndexType indexType_ = indexType(metalBuffer->descriptor().stride);
        [renderCommandEncoder drawIndexedPrimitives:metalPrimitiveType
                               indexCount:indexCount
                               indexType:indexType_
                               indexBuffer:metalIndexBuffer
                               indexBufferOffset:indexBufferOffset
                               instanceCount:instanceCount
                               baseVertex:baseVertex
                               baseInstance:baseInstance];
    }

    void MetalCommandBuffer::setVertexStageBuffer(Buffer* _Nonnull buffer, unsigned int offset, unsigned int atIndex)
    {
        auto* metalBuffer = static_cast<MetalBuffer*>(buffer);
        [renderCommandEncoder setVertexBuffer:metalBuffer->metalBuffer() offset:offset atIndex:atIndex];
    }

    void MetalCommandBuffer::setVertexStageBufferOffset(unsigned int offset, unsigned int atIndex)
    {
        [renderCommandEncoder setVertexBufferOffset:offset atIndex:atIndex];
    }

    void MetalCommandBuffer::setVertexStageBytes(void const* _Nonnull data, unsigned int length, unsigned int atIndex)
    {
        [renderCommandEncoder setVertexBytes:data length:length atIndex:atIndex];
    }

    void MetalCommandBuffer::setFragmentStageTexture(ITexture* _Nonnull texture, unsigned int atIndex)
    {
        auto* metalTexture = static_cast<MetalTexture*>(texture);
        [renderCommandEncoder setFragmentTexture:metalTexture->get() atIndex:atIndex];
    }
}