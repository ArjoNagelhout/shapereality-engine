//
// Created by Arjo Nagelhout on 30/11/2023.
//

#ifndef SHAPEREALITY_BOUNDS_H
#define SHAPEREALITY_BOUNDS_H

#include "vector.h"

namespace math
{
    struct Ray;

    // axis aligned bounding box (AABB)
    struct Bounds final
    {
        constexpr explicit Bounds(vec3 const& center, vec3 const& size);

        constexpr ~Bounds();

        // get the min point of this bounds (equals center - extents)
        [[nodiscard]] constexpr vec3 min() const;

        // get the max point of this bounds (equals center + extents)
        [[nodiscard]] constexpr vec3 max() const;

        // get the center of this bounds
        [[nodiscard]] constexpr vec3 getCenter() const;

        // get the extents of this bounds
        [[nodiscard]] constexpr vec3 getExtents() const;

        // set the min point of this bounds
        constexpr void setMin(vec3 const& min);

        // set the max point of this bounds
        constexpr void setMax(vec3 const& max);

        // set the min and max point of this bounds (this is more performant than setting each individually)
        constexpr void setMinMax(vec3 const& min, vec3 const& max);

        // get the size of this bounds (equals extents * 2)
        [[nodiscard]] constexpr vec3 size() const;

        // get the closest point on the bounds
        // if the provided point is inside the bounds, it will return the provided point
        [[nodiscard]] constexpr vec3 closestPoint(vec3 const& point) const;

        // get whether this bounds contains the given point
        [[nodiscard]] constexpr bool contains(vec3 const& point) const;

        // grow this bounds to include the given point
        constexpr void encapsulate(vec3 const& point);

        // get whether two bounds intersect
        [[nodiscard]] static constexpr bool intersects(Bounds const& lhs, Bounds const& rhs);

        // get whether a bounds and a ray intersect
        [[nodiscard]] static constexpr bool intersects(Bounds const& lhs, Ray const& rhs);

    private:
        vec3 center;
        vec3 extents;
    };
}

#endif //SHAPEREALITY_BOUNDS_H
