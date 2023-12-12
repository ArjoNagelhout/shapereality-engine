//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_render_pipeline_state.h"

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

	MetalRenderPipelineState::MetalRenderPipelineState() = default;

	MetalRenderPipelineState::~MetalRenderPipelineState() = default;
}