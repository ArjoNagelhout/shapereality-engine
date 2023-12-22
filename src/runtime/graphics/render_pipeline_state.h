//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef SHAPEREALITY_RENDER_PIPELINE_STATE_H
#define SHAPEREALITY_RENDER_PIPELINE_STATE_H

#include <string>

#include "graphics.h"

namespace graphics
{
	enum class PrimitiveTopologyType
	{
		Unspecified,
		Point,
		Line,
		Triangle
	};

	enum class TessellationFactorFormat
	{
		Half
	};

	enum class TessellationControlPointIndexType
	{
		None,
		UInt16,
		UInt32
	};

	enum class TessellationPartitionMode
	{
		Pow2,
		Integer,
		FractionalOdd,
		FractionalEven
	};

	enum class TessellationFactorStepFunction
	{
		Constant
	};

	class IShaderFunction;

	// todo: this descriptor is currently very metal specific
	//  and should be generalized to also fit Vulkan
	struct RenderPipelineDescriptor
	{
		// different from RenderPass::ColorAttachmentDescriptor
		struct ColorAttachmentDescriptor
		{
			PixelFormat pixelFormat;
		};

		// identifying the render pipeline state object
		std::string label;

		// graphics functions and associated data
		IShaderFunction* vertexFunction;
		IShaderFunction* fragmentFunction;
		int maxVertexCallstackDepth;
		int maxFragmentCallstackDepth;

		// buffer layout and fetch behavior
		int vertexDescriptor;

		// buffer mutability
		int vertexBuffers;
		int fragmentBuffers;

		// specifying state
		int reset;
		std::vector<ColorAttachmentDescriptor> colorAttachments{};
		//int colorAttachments;
		PixelFormat depthAttachmentPixelFormat;
		PixelFormat stencilAttachmentPixelFormat;

		// rasterization and visibility state
		bool alphaToCoverageEnabled;
		bool alphaToOneEnabled;
		bool rasterizationEnabled;
		PrimitiveTopologyType inputPrimitiveTopology;
		int rasterSampleCount;

		// tessellation state
		int maxTessellationFactor;
		bool tessellationFactorScaleEnabled;
		TessellationFactorFormat tessellationFactorFormat;
		TessellationControlPointIndexType tessellationControlPointIndexType;
		TessellationFactorStepFunction tessellationFactorStepFunction;
		WindingOrder tessellationOutputWindingOrder;
		TessellationPartitionMode tessellationPartitionMode;

		// indirect command buffers usage
		bool supportIndirectCommandBuffers;

		// maximum vertex amplification count
		int maxVertexAmplificationCount;

		// specifying precompiled shader binaries
		bool supportAddingVertexBinaryFunctions;
		bool supportAddingFragmentBinaryFunctions;
		int binaryArchives;

		// callable functions for pipeline
		int vertexLinkedFunctions;
		int fragmentLinkedFunctions;
	};

	class IRenderPipelineState
	{
	public:
		virtual ~IRenderPipelineState() = default;
	};

	struct StencilDescriptor
	{
		// todo
	};

	struct DepthStencilDescriptor
	{
		// depth operations
		CompareFunction depthCompareFunction;
		bool depthWriteEnabled;

		// stencil descriptors for primitives
		// todo
	};

	class IDepthStencilState
	{
	public:
		virtual ~IDepthStencilState() = default;
	};
}

#endif //SHAPEREALITY_RENDER_PIPELINE_STATE_H
