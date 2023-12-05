//
// Created by Arjo Nagelhout on 30/11/2023.
//

#ifndef BORED_ENGINE_RAY_H
#define BORED_ENGINE_RAY_H

#include "vector.h"

#include <string>

namespace math
{
	struct Ray final
	{
		// construct a ray
		// direction is always normalized
		constexpr explicit Ray(vec3 const& origin, vec3 const& direction);
		constexpr ~Ray();

		//
		[[nodiscard]] std::string toString() const;

		// get the point at the given distance from the origin of this ray along this ray
		[[nodiscard]] constexpr vec3 getPoint(float distance) const;

		// get origin, non-mutable
		[[nodiscard]] constexpr vec3 origin() const;

		// get direction, non-mutable
		[[nodiscard]] constexpr vec3 direction() const;

		//
		constexpr void setOrigin(vec3 const& origin);

		// note: normalizes the given direction
		constexpr void setDirection(vec3 const& direction);

	private:
		vec3 _origin;
		vec3 _direction;
	};

	constexpr std::ostream& operator<<(std::ostream& ostream, Ray const& ray);
}

#endif //BORED_ENGINE_RAY_H
