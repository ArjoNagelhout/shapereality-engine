//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef BORED_ENGINE_RAY_INL
#define BORED_ENGINE_RAY_INL

#include "ray.h"
#include "vector.inl"

namespace math
{
	constexpr vec3 Ray::getPoint(float distance) const
	{
		return origin + direction * distance;
	}
}

#endif //BORED_ENGINE_RAY_INL
