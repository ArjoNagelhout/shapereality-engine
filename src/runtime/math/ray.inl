//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef SHAPEREALITY_RAY_INL
#define SHAPEREALITY_RAY_INL

#include "ray.h"
#include "vector.inl"

#include <sstream>

namespace math
{
	constexpr Ray::Ray(vec3 const& origin, vec3 const& direction)
		: _origin(origin), _direction(direction.normalized())
	{
	}

	constexpr Ray::~Ray() = default;

	constexpr vec3 Ray::getPoint(float distance) const
	{
		return _origin + _direction * distance;
	}

	constexpr vec3 Ray::origin() const
	{
		return _origin;
	}

	constexpr vec3 Ray::direction() const
	{
		return _direction;
	}

	constexpr void Ray::setOrigin(vec3 const& origin)
	{
		_origin = origin;
	}

	constexpr void Ray::setDirection(vec3 const& direction)
	{
		_direction = direction.normalized();
	}

	constexpr std::ostream& operator<<(std::ostream& ostream, Ray const& ray)
	{
		ostream << ray.toString();
		return ostream;
	}
}

#endif //SHAPEREALITY_RAY_INL
