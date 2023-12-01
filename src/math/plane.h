//
// Created by Arjo Nagelhout on 30/11/2023.
//

#ifndef BORED_ENGINE_PLANE_H
#define BORED_ENGINE_PLANE_H

#include "vector.h"

namespace math
{
	class Ray;

	// half-plane that divides the space into two halves
	class Plane final
	{
	public:
		// construct a plane with a given normal and through a given point
		// note: the given normal is assumed to be normalized
		constexpr explicit Plane(vec3 const& normal, vec3 const& point);

		// construct a plane with a given normal and with a given distance from the origin (0, 0, 0)
		// note: the given normal is assumed to be normalized
		constexpr explicit Plane(vec3 const& normal, float distance);

		// construct a plane that goes through the provided points a, b and c
		constexpr explicit Plane(vec3 const& a, vec3 const& b, vec3 const& c);

		constexpr ~Plane();

		// get the normal of this plane, non-mutable
		[[nodiscard]] constexpr vec3 normal() const;

		// get the distance of this plane from the origin (0, 0, 0), non-mutable
		[[nodiscard]] constexpr vec3 distance() const;

		//
		constexpr void setNormal();

		//
		constexpr void setDistance();

		// get the signed distance from a given point to this plane
		constexpr float signedDistanceToPoint(vec3 const& point);

		// get the closest point on this plane todo: change into project? as it simply projects the point onto the plane?
		constexpr vec3 closestPointOnPlane(vec3 const& point);

		// get whether this plane and a given ray intersect
		// distance gets set to the distance from the ray origin to the intersection point
		// if no intersection, distance is 0.f
		constexpr bool intersects(Ray const& ray, float& distance) const;

		// get whether a given point is on the positive side of this plane
		constexpr bool isOnPositiveSide(vec3 const& point);

		// get whether two given points a and b are on the same side of this plane
		constexpr bool sameSide(vec3 const& a, vec3 const& b) const;

		//
		constexpr void setFrom3Points(vec3 const& a, vec3 const& b, vec3 const& c);

		//
		constexpr void setNormalAndPoint(vec3 const& normal, vec3 const& point);

		//
		constexpr static Plane translate(vec3 const& translation);

	private:
		vec3 _normal;
		float _distance;
	};
}

#endif //BORED_ENGINE_PLANE_H
