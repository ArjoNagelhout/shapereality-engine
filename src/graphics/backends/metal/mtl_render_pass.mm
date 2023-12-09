//
// Created by Arjo Nagelhout on 09/12/2023.
//

#include "mtl_render_pass.h"

namespace graphics
{
	// convert enums

	MTLStoreAction convert(StoreAction action)
	{
		switch (action)
		{
			case StoreAction::Store: return MTLStoreActionStore;
			case StoreAction::DontCare: return MTLStoreActionDontCare;
			case StoreAction::MultisampleResolve: return MTLStoreActionMultisampleResolve;
		}
	}

	MTLLoadAction convert(LoadAction action)
	{
		switch (action)
		{
			case LoadAction::DontCare: return MTLLoadActionDontCare;
			case LoadAction::Clear: return MTLLoadActionClear;
			case LoadAction::Load: return MTLLoadActionLoad;
		}
	}

	MTLMultisampleDepthResolveFilter convert(MultisampleDepthResolveFilter filter)
	{
		switch (filter)
		{
			case MultisampleDepthResolveFilter::Sample0: return MTLMultisampleDepthResolveFilterSample0;
			case MultisampleDepthResolveFilter::Min: return MTLMultisampleDepthResolveFilterMin;
			case MultisampleDepthResolveFilter::Max: return MTLMultisampleDepthResolveFilterMax;
		}
	}

	void setAttachmentProperties(MTLRenderPassAttachmentDescriptor* attachment, RenderPassDescriptor::AttachmentDescriptor const& source)
	{
		attachment.storeAction = convert(source.storeAction);
		attachment.loadAction = convert(source.loadAction);
		attachment.level = source.mipLevel;
	}

	MetalRenderPass::MetalRenderPass(RenderPassDescriptor descriptor)
	{
		pDescriptor = [MTLRenderPassDescriptor renderPassDescriptor]; // creates default render pass descriptor

		// color attachments
		for (size_t i = 0; i < descriptor.colorAttachments.size(); i++)
		{
			RenderPassDescriptor::ColorAttachmentDescriptor colorSource = descriptor.colorAttachments[i];
			MTLRenderPassColorAttachmentDescriptor* color = [[MTLRenderPassColorAttachmentDescriptor alloc] init];
			setAttachmentProperties(color, colorSource);
			[pDescriptor.colorAttachments setObject:color atIndexedSubscript:i];
		}

		// depth attachment
		RenderPassDescriptor::DepthAttachmentDescriptor depthSource = descriptor.depthAttachment;
		MTLRenderPassDepthAttachmentDescriptor* depth = [[MTLRenderPassDepthAttachmentDescriptor alloc] init];
		setAttachmentProperties(depth, depthSource);
		depth.clearDepth = depthSource.clearDepth;
		depth.depthResolveFilter = convert(depthSource.depthResolveFilter);
		pDescriptor.depthAttachment = depth;

		// stencil attachment
		RenderPassDescriptor::StencilAttachmentDescriptor stencilSource = descriptor.stencilAttachment;
		MTLRenderPassStencilAttachmentDescriptor* stencil = [[MTLRenderPassStencilAttachmentDescriptor alloc] init];
		setAttachmentProperties(stencil, stencilSource);
		stencil.clearStencil = stencilSource.clearStencil;
		pDescriptor.stencilAttachment = stencil;
	}

	MetalRenderPass::~MetalRenderPass()
	{
		[pDescriptor release];
	}

	MetalRenderPass::MetalRenderPass(MTLRenderPassDescriptor* descriptor)
	{
		pDescriptor = descriptor;
	}
}