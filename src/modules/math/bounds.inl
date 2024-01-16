//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef SHAPEREALITY_BOUNDS_INL
#define SHAPEREALITY_BOUNDS_INL

#include "bounds.h"
#include "vector.inl"

namespace math
{
    constexpr Bounds::Bounds(Vector3 const& center, Vector3 const& size)
        : center(center), extents(size / 2)
    {
    }

    constexpr Bounds::~Bounds() = default;

    constexpr Vector3 Bounds::min() const
    {
        return center - extents;
    }

    constexpr Vector3 Bounds::max() const
    {
        return center + extents;
    }

    constexpr Vector3 Bounds::getCenter() const
    {
        return center;
    }

    constexpr Vector3 Bounds::getExtents() const
    {
        return extents;
    }

    constexpr void Bounds::setMin(Vector3 const& min)
    {
        // todo
    }

    constexpr void Bounds::setMax(Vector3 const& max)
    {
        // todo
    }

    constexpr void Bounds::setMinMax(Vector3 const& min, Vector3 const& max)
    {
        center = (min + max) / 2.f;
        // todo: _extents =
    }

    constexpr Vector3 Bounds::size() const
    {
        return extents * 2;
    }

    constexpr Vector3 Bounds::closestPoint(Vector3 const& point) const
    {
        // todo
        return point;
    }

    constexpr bool Bounds::contains(Vector3 const& point) const
    {
        // todo
        return false;
    }

    constexpr void Bounds::encapsulate(Vector3 const& point)
    {
        // todo
    }

    constexpr bool Bounds::intersects(Bounds const& lhs, Bounds const& rhs)
    {
        // todo
        return false;
    }

    constexpr bool Bounds::intersects(Bounds const& lhs, Ray const& rhs)
    {
        // todo
        return false;
    }
}

#endif //SHAPEREALITY_BOUNDS_INL
