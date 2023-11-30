//
// Created by Arjo Nagelhout on 30/11/2023.
//

#ifndef BORED_ENGINE_BOUNDS_H
#define BORED_ENGINE_BOUNDS_H

#include "vector.h"

namespace math
{
	class Ray;

	// axis aligned bounding box (AABB)
	class Bounds
	{
	public:
		explicit Bounds(vec3 const& center, vec3 const& extents);
		~Bounds();

		// get the min point of this bounds (equals center - extents)
		[[nodiscard]] constexpr vec3 min() const;

		// get the max point of this bounds (equals center + extents)
		[[nodiscard]] constexpr vec3 max() const;

		// get the size of this bounds (equals extents * 2)
		[[nodiscard]] constexpr vec3 size() const;

		// get the closest point on the bounds
		// if the provided point is inside the bounds, it will return the provided point
		[[nodiscard]] static constexpr vec3 closestPoint(vec3 const& point);

		// get whether two bounds intersect
		[[nodiscard]] static constexpr vec3 intersects(Bounds const& lhs, Bounds const& rhs);

		// get whether a bounds and a ray intersect
		[[nodiscard]] static constexpr vec3 intersects(Bounds const& lhs, Ray const& rhs);

	private:
		vec3 center;
		vec3 extents;
	};
}

#endif //BORED_ENGINE_BOUNDS_H
