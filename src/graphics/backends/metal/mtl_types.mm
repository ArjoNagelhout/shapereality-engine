//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_types.h"

namespace graphics
{
	MTLWinding convert(WindingOrder windingOrder)
	{
		switch (windingOrder)
		{
			case WindingOrder::Clockwise: return MTLWindingClockwise;
			case WindingOrder::CounterClockwise: return MTLWindingCounterClockwise;
		}
	}

	MTLPrimitiveType convert(PrimitiveType primitiveType)
	{
		switch (primitiveType)
		{
			case PrimitiveType::Point: return MTLPrimitiveTypePoint;
			case PrimitiveType::Line: return MTLPrimitiveTypeLine;
			case PrimitiveType::LineStrip: return MTLPrimitiveTypeLineStrip;
			case PrimitiveType::Triangle: return MTLPrimitiveTypeTriangle;
			case PrimitiveType::TriangleStrip: return MTLPrimitiveTypeTriangleStrip;
		}
	}
}