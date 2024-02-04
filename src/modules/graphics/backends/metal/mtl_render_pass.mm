//
// Created by Arjo Nagelhout on 09/12/2023.
//

#include "mtl_render_pass.h"
#include "mtl_types.h"
#include "mtl_texture.h"

namespace graphics::metal
{
    // from metal to platform-agnostic
    void setAttachmentProperties(RenderPassDescriptor::AttachmentDescriptor& attachment,
                                 MTLRenderPassAttachmentDescriptor* source)
    {
        if (source.texture != nullptr)
        {
            attachment.texture = std::make_unique<MetalTexture>(source.texture);
        }
        attachment.storeAction = convert(source.storeAction);
        attachment.loadAction = convert(source.loadAction);
        attachment.mipmapLevel = source.level;
    }

    // from platform-agnostic to metal
    void setAttachmentProperties(MTLRenderPassAttachmentDescriptor* attachment,
                                 RenderPassDescriptor::AttachmentDescriptor const& source)
    {
        if (source.texture != nullptr)
        {
            attachment.texture = static_cast<MetalTexture*>(source.texture.get())->get();
        }
        attachment.storeAction = convert(source.storeAction);
        attachment.loadAction = convert(source.loadAction);
        attachment.level = source.mipmapLevel;
    }

    std::unique_ptr<RenderPassDescriptor> createRenderPassDescriptor(MTLRenderPassDescriptor* _Nonnull descriptor)
    {
        std::unique_ptr<RenderPassDescriptor> result = std::make_unique<RenderPassDescriptor>();

        unsigned int i = 0;
        auto* colorSource = [descriptor.colorAttachments objectAtIndexedSubscript:i];
        while (colorSource && colorSource.texture) // if a color attachment does not contain a texture, we can assume we don't need it
        {
            RenderPassDescriptor::ColorAttachmentDescriptor color;
            color.clearColor = convert(colorSource.clearColor);
            setAttachmentProperties(color, colorSource);
            result->colorAttachments.emplace_back(std::move(color));

            // next color source
            i++;
            colorSource = [descriptor.colorAttachments objectAtIndexedSubscript:i];
        }

        MTLRenderPassDepthAttachmentDescriptor* depthSource = descriptor.depthAttachment;
        if (depthSource && depthSource.texture)
        {
            RenderPassDescriptor::DepthAttachmentDescriptor depth;
            depth.clearDepth = static_cast<float>(depthSource.clearDepth);
            setAttachmentProperties(depth, depthSource);
            result->depthAttachment = std::move(depth);
        }

        MTLRenderPassStencilAttachmentDescriptor* stencilSource = descriptor.stencilAttachment;
        if (stencilSource && stencilSource.texture)
        {
            RenderPassDescriptor::StencilAttachmentDescriptor stencil;
            stencil.clearStencil = stencilSource.clearStencil;
            setAttachmentProperties(stencil, stencilSource);
            result->stencilAttachment = std::move(stencil);
        }

        return result;
    }

    MTLRenderPassDescriptor* _Nonnull createRenderPassDescriptor(RenderPassDescriptor const& descriptor)
    {
        MTLRenderPassDescriptor* result = [MTLRenderPassDescriptor renderPassDescriptor]; // creates default render pass descriptor

        // color attachments
        for (size_t i = 0; i < descriptor.colorAttachments.size(); i++)
        {
            RenderPassDescriptor::ColorAttachmentDescriptor const& colorSource = descriptor.colorAttachments[i];
            MTLRenderPassColorAttachmentDescriptor* color = [[MTLRenderPassColorAttachmentDescriptor alloc] init];
            color.clearColor = convert(colorSource.clearColor);
            setAttachmentProperties(color, colorSource);
            [result.colorAttachments setObject:color atIndexedSubscript:i];
        }

        // depth attachment
        RenderPassDescriptor::DepthAttachmentDescriptor const& depthSource = descriptor.depthAttachment;
        MTLRenderPassDepthAttachmentDescriptor* depth = [[MTLRenderPassDepthAttachmentDescriptor alloc] init];
        setAttachmentProperties(depth, depthSource);
        depth.clearDepth = depthSource.clearDepth;
        depth.depthResolveFilter = convert(depthSource.depthResolveFilter);
        result.depthAttachment = depth;

        // stencil attachment
        RenderPassDescriptor::StencilAttachmentDescriptor const& stencilSource = descriptor.stencilAttachment;
        MTLRenderPassStencilAttachmentDescriptor* stencil = [[MTLRenderPassStencilAttachmentDescriptor alloc] init];
        setAttachmentProperties(stencil, stencilSource);
        stencil.clearStencil = stencilSource.clearStencil;
        result.stencilAttachment = stencil;

        return result;
    }
}