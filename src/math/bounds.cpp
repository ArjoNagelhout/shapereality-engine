//
// Created by Arjo Nagelhout on 30/11/2023.
//

#include "bounds.h"
#include "vector.h"
#include "vector.inl"

namespace math
{
	Bounds::Bounds(vec3 const& center, vec3 const& extents)
		: center(center), extents(extents)
	{
	}

	Bounds::~Bounds() = default;

	constexpr vec3 Bounds::min() const
	{
		return center - extents;
	}

	constexpr vec3 Bounds::max() const
	{
		return center + extents;
	}
}