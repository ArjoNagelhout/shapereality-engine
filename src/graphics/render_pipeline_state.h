//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef BORED_ENGINE_RENDER_PIPELINE_STATE_H
#define BORED_ENGINE_RENDER_PIPELINE_STATE_H

#include <string>
#import "types.h"

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

	// todo: this descriptor is currently
	// very metal specific and should be generalized to
	// also fit Vulkan and other computer architectures.
	struct RenderPipelineDescriptor
	{
		// identifying the render pipeline state object
		std::string label;

		// graphics functions and associated data
		int vertexFunction;
		int fragmentFunction;
		int maxVertexCallstackDepth;
		int maxFragmentCallstackDepth;

		// buffer layout and fetch behavior
		int vertexDescriptor;

		// buffer mutability
		int vertexBuffers;
		int fragmentBuffers;

		// specifying state
		int reset;
		int colorAttachments;
		int depthAttachmentPixelFormat;
		int stencilAttachmentPixelFormat;

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
		Winding tessellationOutputWindingOrder;
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
}

#endif //BORED_ENGINE_RENDER_PIPELINE_STATE_H
