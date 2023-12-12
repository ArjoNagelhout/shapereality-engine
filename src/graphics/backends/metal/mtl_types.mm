//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_types.h"

namespace graphics
{
	MTLWinding convert(Winding winding)
	{
		switch (winding)
		{
			case Winding::Clockwise: return MTLWindingClockwise;
			case Winding::CounterClockwise: return MTLWindingCounterClockwise;
		}
	}
}