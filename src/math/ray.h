//
// Created by Arjo Nagelhout on 30/11/2023.
//

#ifndef BORED_ENGINE_RAY_H
#define BORED_ENGINE_RAY_H

#include "vector.h"

#include <string>

namespace math
{
	class Ray final
	{
	public:
		// construct a ray
		// direction is always normalized
		constexpr explicit Ray(vec3 const& origin, vec3 const& direction);
		constexpr ~Ray();

		// get the point at the given distance from the origin of this ray along this ray
		[[nodiscard]] constexpr vec3 getPoint(float distance) const;

		[[nodiscard]] std::string toString() const;

		vec3 const origin;
		vec3 const direction;
	};

	constexpr std::ostream& operator<<(std::ostream& ostream, Ray const& ray);
}

#endif //BORED_ENGINE_RAY_H
