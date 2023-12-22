//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef SHAPEREALITY_PLANE_INL
#define SHAPEREALITY_PLANE_INL

#include "plane.h"

namespace math
{
	constexpr Plane::Plane(vec3 const& normal, vec3 const& point)
		: _normal(normal), _distance(1.f)
	{
	}
}

#endif //SHAPEREALITY_PLANE_INL
