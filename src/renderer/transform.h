//
// Created by Arjo Nagelhout on 16/12/2023.
//

#ifndef BORED_ENGINE_TRANSFORM_H
#define BORED_ENGINE_TRANSFORM_H

#include "math/vector.h"
#include "math/quaternion.h"
#include "math/matrix.h"

namespace renderer
{
	// first do a simple transform hierarchy implementation.
	// Later we can make it more complicated with an entity component system implementation
	class Transform final
	{
	public:
		explicit Transform();
		explicit Transform(math::vec3 localPosition, math::Quaternion localRotation, math::vec3 localScale);
		~Transform();

	private:

		math::vec3 localPosition;
		math::Quaternion localRotation;
		math::vec3 localScale;
	};
}

#endif //BORED_ENGINE_TRANSFORM_H
