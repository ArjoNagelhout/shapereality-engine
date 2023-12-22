//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef BORED_ENGINE_BOUNDS_INL
#define BORED_ENGINE_BOUNDS_INL

#include "bounds.h"
#include "vector.inl"

namespace math
{
	constexpr Bounds::Bounds(vec3 const& center, vec3 const& size)
		: _center(center), _extents(size / 2)
	{
	}

	constexpr Bounds::~Bounds() = default;

	constexpr vec3 Bounds::min() const
	{
		return _center - _extents;
	}

	constexpr vec3 Bounds::max() const
	{
		return _center + _extents;
	}

	constexpr vec3 Bounds::center() const
	{
		return _center;
	}

	constexpr vec3 Bounds::extents() const
	{
		return _extents;
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
		_center = (min + max) / 2.f;
		// todo: _extents =
	}

	constexpr vec3 Bounds::size() const
	{
		return _extents * 2;
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

#endif //BORED_ENGINE_BOUNDS_INL
