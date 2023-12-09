//
// Created by Arjo Nagelhout on 09/12/2023.
//

#include "types.h"

namespace graphics
{
	Color::Color(float r, float g, float b, float a)
		: r(r), g(g), b(b), a(a)
	{

	}

	Color::~Color() = default;
}