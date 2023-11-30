//
// Created by Arjo Nagelhout on 30/11/2023.
//

#ifndef BORED_ENGINE_RAY_H
#define BORED_ENGINE_RAY_H

#include "vector.h"

namespace math
{
	class Ray final
	{
	public:
		explicit Ray(vec3 const& origin, vec3 const& direction);
		~Ray();

	private:
	};
}

#endif //BORED_ENGINE_RAY_H
