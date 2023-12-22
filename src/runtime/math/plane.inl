//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef BORED_ENGINE_PLANE_INL
#define BORED_ENGINE_PLANE_INL

#include "plane.h"

namespace math
{
	constexpr Plane::Plane(vec3 const& normal, vec3 const& point)
		: _normal(normal), _distance(1.f)
	{
	}
}

#endif //BORED_ENGINE_PLANE_INL
