//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef SHAPEREALITY_BOUNDS_INL
#define SHAPEREALITY_BOUNDS_INL

#include "bounds.h"
#include "vector.inl"

namespace math
{
	constexpr Bounds::Bounds(vec3 const& center, vec3 const& size)
		: center(center), extents(size / 2)
	{
	}

	constexpr Bounds::~Bounds() = default;

	constexpr vec3 Bounds::min() const
	{
		return center - extents;
	}

	constexpr vec3 Bounds::max() const
	{
		return center + extents;
	}

	constexpr vec3 Bounds::getCenter() const
	{
		return center;
	}

	constexpr vec3 Bounds::getExtents() const
	{
		return extents;
	}

	constexpr void Bounds::setMin(vec3 const& min)
	{
		// todo
	}

	constexpr void Bounds::setMax(vec3 const& max)
	{
		// todo
	}

	constexpr void Bounds::setMinMax(vec3 const& min, vec3 const& max)
	{
		center = (min + max) / 2.f;
		// todo: _extents =
	}

	constexpr vec3 Bounds::size() const
	{
		return extents * 2;
	}

	constexpr vec3 Bounds::closestPoint(vec3 const& point) const
	{
		// todo
		return point;
	}

	constexpr bool Bounds::contains(vec3 const& point) const
	{
		// todo
		return false;
	}

	constexpr void Bounds::encapsulate(vec3 const& point)
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
