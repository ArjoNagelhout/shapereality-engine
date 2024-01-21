//
// Created by Arjo Nagelhout on 09/12/2023.
//

#include "mtl_render_pass.h"
#include "mtl_types.h"
#include "mtl_texture.h"

namespace graphics
{
    // from metal to platform-agnostic
    void setAttachmentProperties(RenderPassDescriptor::AttachmentDescriptor& attachment,
                                 MTLRenderPassAttachmentDescriptor* source)
    {
        if (source.texture != nullptr)
        {
            attachment.pTexture = std::make_unique<MetalTexture>(source.texture);
        }
        attachment.storeAction = convert(source.storeAction);
        attachment.loadAction = convert(source.loadAction);
        attachment.mipmapLevel = source.level;
    }

    // from platform-agnostic to metal
    void setAttachmentProperties(MTLRenderPassAttachmentDescriptor* attachment,
                                 RenderPassDescriptor::AttachmentDescriptor const& source)
    {
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

        }

        return result;
    }

    MetalRenderPass::MetalRenderPass(id <MTLDevice> _Nonnull pDevice, RenderPassDescriptor const& descriptor)
    {
        pDescriptor = [MTLRenderPassDescriptor renderPassDescriptor]; // creates default render pass descriptor

        // color attachments
        for (size_t i = 0; i < descriptor.colorAttachments.size(); i++)
        {
            RenderPassDescriptor::ColorAttachmentDescriptor const& colorSource = descriptor.colorAttachments[i];
            MTLRenderPassColorAttachmentDescriptor* color = [[MTLRenderPassColorAttachmentDescriptor alloc] init];
            color.clearColor = convert(colorSource.clearColor);
            setAttachmentProperties(color, colorSource);
            [pDescriptor.colorAttachments setObject:color atIndexedSubscript:i];
        }

        // depth attachment
        RenderPassDescriptor::DepthAttachmentDescriptor const& depthSource = descriptor.depthAttachment;
        MTLRenderPassDepthAttachmentDescriptor* depth = [[MTLRenderPassDepthAttachmentDescriptor alloc] init];
        setAttachmentProperties(depth, depthSource);
        depth.clearDepth = depthSource.clearDepth;
        depth.depthResolveFilter = convert(depthSource.depthResolveFilter);
        pDescriptor.depthAttachment = depth;

        // stencil attachment
        RenderPassDescriptor::StencilAttachmentDescriptor const& stencilSource = descriptor.stencilAttachment;
        MTLRenderPassStencilAttachmentDescriptor* stencil = [[MTLRenderPassStencilAttachmentDescriptor alloc] init];
        setAttachmentProperties(stencil, stencilSource);
        stencil.clearStencil = stencilSource.clearStencil;
        pDescriptor.stencilAttachment = stencil;

        // todo: create (if textures are not set, we should create them here, otherwise simply pass the textures along)
    }

    MetalRenderPass::~MetalRenderPass()
    {
        [pDescriptor release];
    }

    MetalRenderPass::MetalRenderPass(MTLRenderPassDescriptor* descriptor)
    {
        pDescriptor = descriptor;
        [pDescriptor retain]; // otherwise the caller of this constructor is still responsible for holding this descriptor
    }
}