//
// Created by Arjo Nagelhout on 30/11/2023.
//

#include "ray.h"
#include "vector.inl"

#include <sstream>

namespace math
{
	Ray::Ray(vec3 const& origin, vec3 const& direction)
		: origin(origin), direction(direction.normalized())
	{
	}

	Ray::~Ray() = default;

	std::string Ray::toString() const
	{
		std::stringstream result{};
		result << "{origin: " << origin << ", direction: " << direction << "}";
		return result.str();
	}

	std::ostream& operator<<(std::ostream& ostream, Ray const& ray)
	{
		ostream << ray.toString();
		return ostream;
	}
}