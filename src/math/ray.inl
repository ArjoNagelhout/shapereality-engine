//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef BORED_ENGINE_RAY_INL
#define BORED_ENGINE_RAY_INL

#include "ray.h"
#include "vector.inl"

#include <sstream>

namespace math
{
	constexpr Ray::Ray(vec3 const& origin, vec3 const& direction)
		: origin(origin), direction(direction.normalized())
	{
	}

	constexpr Ray::~Ray() = default;

	constexpr vec3 Ray::getPoint(float distance) const
	{
		return origin + direction * distance;
	}

	constexpr std::ostream& operator<<(std::ostream& ostream, Ray const& ray)
	{
		ostream << ray.toString();
		return ostream;
	}
}

#endif //BORED_ENGINE_RAY_INL
