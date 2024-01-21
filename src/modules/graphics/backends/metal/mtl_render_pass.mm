//
// Created by Arjo Nagelhout on 09/12/2023.
//

#include "mtl_render_pass.h"
#include "mtl_types.h"
#include "mtl_texture.h"

namespace graphics
{
    // from platform-agnostic to metal
    void setAttachmentProperties(MTLRenderPassAttachmentDescriptor* attachment,
                                 RenderPassDescriptor::AttachmentDescriptor const& source)
    {
        attachment.storeAction = convert(source.storeAction);
        attachment.loadAction = convert(source.loadAction);
        attachment.level = source.mipmapLevel;
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

        [pDescriptor retain];
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