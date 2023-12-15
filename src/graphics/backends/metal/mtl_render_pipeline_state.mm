//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_render_pipeline_state.h"
#import "mtl_shader.h"

#include "mtl_utils.h"
#include "mtl_types.h"

namespace graphics
{
	MTLPrimitiveTopologyClass convert(PrimitiveTopologyType type)
	{
		switch (type)
		{
			case PrimitiveTopologyType::Unspecified: return MTLPrimitiveTopologyClassUnspecified;
			case PrimitiveTopologyType::Point: return MTLPrimitiveTopologyClassPoint;
			case PrimitiveTopologyType::Line: return MTLPrimitiveTopologyClassLine;
			case PrimitiveTopologyType::Triangle: return MTLPrimitiveTopologyClassTriangle;
		}
	}

	MTLTessellationFactorFormat convert(TessellationFactorFormat format)
	{
		switch (format)
		{
			case TessellationFactorFormat::Half: return MTLTessellationFactorFormatHalf;
		}
	}

	MTLTessellationPartitionMode convert(TessellationPartitionMode mode)
	{
		switch (mode)
		{
			case TessellationPartitionMode::Pow2: return MTLTessellationPartitionModePow2;
			case TessellationPartitionMode::Integer: return MTLTessellationPartitionModeInteger;
			case TessellationPartitionMode::FractionalOdd: return MTLTessellationPartitionModeFractionalOdd;
			case TessellationPartitionMode::FractionalEven: return MTLTessellationPartitionModeFractionalEven;
		}
	}

	MetalRenderPipelineState::MetalRenderPipelineState(RenderPipelineDescriptor const& descriptor, id<MTLDevice> _Nonnull pDevice)
	{
		MTLRenderPipelineDescriptor* metalDescriptor = [[MTLRenderPipelineDescriptor alloc] init];

		auto* metalVertexFunction = dynamic_cast<MetalShaderFunction*>(descriptor.vertexFunction);
		auto* metalFragmentFunction = dynamic_cast<MetalShaderFunction*>(descriptor.fragmentFunction);

		metalDescriptor.vertexFunction = metalVertexFunction->getFunction();
		metalDescriptor.fragmentFunction = metalFragmentFunction->getFunction();

		for (size_t i = 0; i < descriptor.colorAttachments.size(); i++)
		{
			auto colorAttachmentDescriptor = descriptor.colorAttachments[i];
			MTLRenderPipelineColorAttachmentDescriptor* color = [[MTLRenderPipelineColorAttachmentDescriptor alloc] init];
			color.pixelFormat = convert(colorAttachmentDescriptor.pixelFormat);
			[metalDescriptor.colorAttachments setObject:color atIndexedSubscript:i];
		}

		metalDescriptor.depthAttachmentPixelFormat = convert(descriptor.depthAttachmentPixelFormat);
		metalDescriptor.stencilAttachmentPixelFormat = convert(descriptor.stencilAttachmentPixelFormat);

		NSError* error = nullptr;
		pRenderPipelineState = [pDevice newRenderPipelineStateWithDescriptor:metalDescriptor error:&error];
		checkMetalError(error, "failed to create MTLRenderPipelineState");

		[pRenderPipelineState retain];

		[metalDescriptor release];
	}

	MetalRenderPipelineState::~MetalRenderPipelineState()
	{
		[pRenderPipelineState release];
	}

	id <MTLRenderPipelineState> _Nonnull MetalRenderPipelineState::get() const
	{
		return pRenderPipelineState;
	}

	MetalDepthStencilState::MetalDepthStencilState(DepthStencilDescriptor const& descriptor,
												   id<MTLDevice> _Nonnull pDevice)
	{
		MTLDepthStencilDescriptor* metalDescriptor = [[MTLDepthStencilDescriptor alloc] init];

		metalDescriptor.depthCompareFunction = convert(descriptor.depthCompareFunction);
		metalDescriptor.depthWriteEnabled = descriptor.depthWriteEnabled;

		pDepthStencilState = [pDevice newDepthStencilStateWithDescriptor:metalDescriptor];
		[pDepthStencilState retain];
	}

	MetalDepthStencilState::~MetalDepthStencilState()
	{
		[pDepthStencilState release];
	}

	id<MTLDepthStencilState> _Nonnull MetalDepthStencilState::get() const
	{
		return pDepthStencilState;
	}
}