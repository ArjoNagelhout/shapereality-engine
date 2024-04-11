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
        activeCommandEncoderType = CommandEncoderType::Render;
        activeCommandEncoder = renderCommandEncoder;
    }

    void MetalCommandBuffer::endRenderPass()
    {
        ensureEncoder(CommandEncoderType::Render);

        [renderCommandEncoder endEncoding];
        [activeCommandEncoder release];
        activeCommandEncoderType = CommandEncoderType::None;
        activeCommandEncoder = nullptr;
    }

    void MetalCommandBuffer::present(ITexture* texture)
    {
        auto metalTexture = static_cast<MetalTexture*>(texture);
        [commandBuffer presentDrawable:metalTexture->getDrawable()];
    }

    void MetalCommandBuffer::enqueue()
    {
        assert(false && "enqueue() is not supported");
    }

    void MetalCommandBuffer::commit()
    {
        if (activeCommandEncoder != nullptr)
        {
            [activeCommandEncoder endEncoding];
        }

        [commandBuffer commit];

        if (activeCommandEncoder != nullptr)
        {
            [activeCommandEncoder release];
            activeCommandEncoder = nullptr;
            activeCommandEncoderType = CommandEncoderType::None;
        }
    }

    void MetalCommandBuffer::setRenderPipelineState(IRenderPipelineState* renderPipelineState)
    {
        ensureEncoder(CommandEncoderType::Render);
        auto* metalPipelineState = static_cast<MetalRenderPipelineState*>(renderPipelineState);
        [renderCommandEncoder setRenderPipelineState:metalPipelineState->get()];
    }

    void MetalCommandBuffer::setDepthStencilState(IDepthStencilState* depthStencilState)
    {
        ensureEncoder(CommandEncoderType::Render);
        auto* metalDepthStencilState = static_cast<MetalDepthStencilState*>(depthStencilState);
        [renderCommandEncoder setDepthStencilState:metalDepthStencilState->get()];
    }

    void MetalCommandBuffer::setWindingOrder(WindingOrder windingOrder)
    {
        ensureEncoder(CommandEncoderType::Render);
        [renderCommandEncoder setFrontFacingWinding:convert(windingOrder)];
    }

    void MetalCommandBuffer::setCullMode(CullMode cullMode)
    {
        ensureEncoder(CommandEncoderType::Render);
        [renderCommandEncoder setCullMode:convert(cullMode)];
    }

    void MetalCommandBuffer::setTriangleFillMode(TriangleFillMode triangleFillMode)
    {
        ensureEncoder(CommandEncoderType::Render);
        [renderCommandEncoder setTriangleFillMode:convert(triangleFillMode)];
    }

    void MetalCommandBuffer::setViewport(Viewport viewport)
    {
        ensureEncoder(CommandEncoderType::Render);
        [renderCommandEncoder setViewport:convert(viewport)];
    }

    void MetalCommandBuffer::setScissorRect(ScissorRect scissorRect)
    {
        ensureEncoder(CommandEncoderType::Render);
        [renderCommandEncoder setScissorRect:convert(scissorRect)];
    }

    void MetalCommandBuffer::drawPrimitives(PrimitiveType primitiveType, unsigned int vertexStart, unsigned int vertexCount)
    {
        ensureEncoder(CommandEncoderType::Render);
        MTLPrimitiveType metalPrimitiveType = convert(primitiveType);
        [renderCommandEncoder drawPrimitives:metalPrimitiveType
                              vertexStart:vertexStart
                              vertexCount:vertexCount];
    }

    void MetalCommandBuffer::drawIndexedPrimitives(PrimitiveType primitiveType,
                                                   unsigned int indexCount,
                                                   Buffer* _Nonnull indexBuffer,
                                                   unsigned int indexBufferOffset,
                                                   unsigned int instanceCount,
                                                   unsigned int baseVertex,
                                                   unsigned int baseInstance)
    {
        ensureEncoder(CommandEncoderType::Render);
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
        ensureEncoder(CommandEncoderType::Render);
        auto* metalBuffer = static_cast<MetalBuffer*>(buffer);
        [renderCommandEncoder setVertexBuffer:metalBuffer->metalBuffer() offset:offset atIndex:atIndex];
    }

    void MetalCommandBuffer::setVertexStageBufferOffset(unsigned int offset, unsigned int atIndex)
    {
        ensureEncoder(CommandEncoderType::Render);
        [renderCommandEncoder setVertexBufferOffset:offset atIndex:atIndex];
    }

    void MetalCommandBuffer::setVertexStageBytes(void const* _Nonnull data, unsigned int length, unsigned int atIndex)
    {
        ensureEncoder(CommandEncoderType::Render);
        [renderCommandEncoder setVertexBytes:data length:length atIndex:atIndex];
    }

    void MetalCommandBuffer::setFragmentStageTexture(ITexture* _Nonnull texture, unsigned int atIndex)
    {
        ensureEncoder(CommandEncoderType::Render);
        auto* metalTexture = static_cast<MetalTexture*>(texture);
        [renderCommandEncoder setFragmentTexture:metalTexture->get() atIndex:atIndex];
    }

    void MetalCommandBuffer::copyBuffer(
        Buffer* _Nonnull source,
        size_t sourceOffset,
        Buffer* _Nonnull destination,
        size_t destinationOffset,
        size_t size)
    {
        ensureEncoder(CommandEncoderType::Blit);

        id <MTLBuffer> metalSource = static_cast<MetalBuffer*>(source)->metalBuffer();
        id <MTLBuffer> metalDestination = static_cast<MetalBuffer*>(destination)->metalBuffer();

        [blitCommandEncoder copyFromBuffer:metalSource
                            sourceOffset:sourceOffset
                            toBuffer:metalDestination
                            destinationOffset:destinationOffset
                            size:size];
    }

    void MetalCommandBuffer::ensureEncoder(CommandEncoderType type)
    {
        assert(type != CommandEncoderType::None && "not intended to be used this way");

        if (type == activeCommandEncoderType)
        {
            return; // nothing to do, is already correct encoder type
        }

        // assumes enum is representative of state, does not check if encoders themselves are nullptr
        if (activeCommandEncoderType != CommandEncoderType::None)
        {
            // end encoding
            [activeCommandEncoder endEncoding];
            [activeCommandEncoder release];
            activeCommandEncoder = nullptr;
        }

        // set to correct command encoder
        switch (type)
        {
            case CommandEncoderType::Render:
            {
                assert(false && "render command encoder should be created with beginRenderPass()");
                break;
            }
            case CommandEncoderType::Blit:
            {
                blitCommandEncoder = [commandBuffer blitCommandEncoder]; // with default settings. todo: support setting descriptor / configuration
                [blitCommandEncoder retain];
                activeCommandEncoder = blitCommandEncoder;
                break;
            }
            default:
            {
                assert(false && "unsupported command encoder type");
            }
        }

        activeCommandEncoderType = type;
    }
}