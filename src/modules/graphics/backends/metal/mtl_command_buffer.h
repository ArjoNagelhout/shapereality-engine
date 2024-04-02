//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef SHAPEREALITY_MTL_COMMAND_BUFFER_H
#define SHAPEREALITY_MTL_COMMAND_BUFFER_H

#include <graphics/command_buffer.h>
#include <graphics/buffer.h>

#import <Metal/Metal.h>

namespace graphics::metal
{
    class MetalCommandBuffer final : public ICommandBuffer
    {
    public:
        enum class CommandEncoderType
        {
            None = 0,
            Render,
            Blit
        };

        explicit MetalCommandBuffer(id <MTLCommandBuffer> _Nonnull _pCommandBuffer);

        ~MetalCommandBuffer() override;

        void beginRenderPass(RenderPassDescriptor const& renderPassDescriptor) override;

        void endRenderPass() override;

        void present(ITexture* _Nonnull texture) override;

        void enqueue() override;

        void commit() override;

        void setRenderPipelineState(IRenderPipelineState* _Nonnull renderPipelineState) override;

        void setDepthStencilState(IDepthStencilState* _Nonnull depthStencilState) override;

        void setWindingOrder(WindingOrder windingOrder) override;

        void setCullMode(CullMode cullMode) override;

        void setTriangleFillMode(TriangleFillMode triangleFillMode) override;

        void setViewport(Viewport viewport) override;

        void setScissorRect(ScissorRect scissorRect) override;

        void drawIndexedPrimitives(
            PrimitiveType primitiveType,
            unsigned int indexCount,
            Buffer* _Nonnull indexBuffer,
            unsigned int indexBufferOffset,
            unsigned int instanceCount,
            unsigned int baseVertex,
            unsigned int baseInstance) override;

        void setVertexStageBuffer(Buffer* _Nonnull pBuffer, unsigned int offset, unsigned int atIndex) override;

        void setVertexStageBufferOffset(unsigned int offset, unsigned int atIndex) override;

        void setVertexStageBytes(void const* _Nonnull data, unsigned int length, unsigned int atIndex) override;

        void setFragmentStageTexture(ITexture* _Nonnull texture, unsigned int atIndex) override;

        void copyBuffer(
            Buffer* _Nonnull source,
            size_t sourceOffset,
            Buffer* _Nonnull destination,
            size_t destinationOffset,
            size_t size) override;

    private:
        id <MTLCommandBuffer> _Nonnull commandBuffer;

        CommandEncoderType activeCommandEncoderType = CommandEncoderType::None;
        id <MTLCommandEncoder> _Nullable activeCommandEncoder = nullptr;
        id <MTLRenderCommandEncoder> _Nullable renderCommandEncoder = nullptr;
        id <MTLBlitCommandEncoder> _Nullable blitCommandEncoder = nullptr;

        // automatically switches encoder and creates encoder if needed
        // calls endEncoding on activeCommandEncoder is activeEncoderType is not None
        void ensureEncoder(CommandEncoderType type);
    };
}

#endif //SHAPEREALITY_MTL_COMMAND_BUFFER_H
