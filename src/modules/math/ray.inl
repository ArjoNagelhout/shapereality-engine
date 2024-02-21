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
    constexpr Ray::Ray(Vector3 const& origin, Vector3 const& direction)
        : _origin(origin), _direction(direction.normalized())
    {
    }

    constexpr Ray::~Ray() = default;

    constexpr Vector3 Ray::getPoint(float distance) const
    {
        return _origin + _direction * distance;
    }

    constexpr Vector3 Ray::origin() const
    {
        return _origin;
    }

    constexpr Vector3 Ray::direction() const
    {
        return _direction;
    }

    constexpr void Ray::setOrigin(Vector3 const& origin)
    {
        _origin = origin;
    }

    constexpr void Ray::setDirection(Vector3 const& direction)
    {
        _direction = direction.normalized();
    }

    constexpr std::ostream& operator<<(std::ostream& ostream, Ray const& ray)
    {
        ostream << ray.string();
        return ostream;
    }
}

#endif //SHAPEREALITY_RAY_INL
