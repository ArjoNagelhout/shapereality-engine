//
// Created by Arjo Nagelhout on 30/11/2023.
//

#ifndef BORED_ENGINE_PLANE_H
#define BORED_ENGINE_PLANE_H

#include "vector.h"

namespace math
{
	//
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

	private:
		vec3 _normal;
		float _distance;
	};
}

#endif //BORED_ENGINE_PLANE_H
