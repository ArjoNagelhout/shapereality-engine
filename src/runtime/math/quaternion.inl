#include "quaternion.h"

#ifndef SHAPEREALITY_QUATERNION_INL
#define SHAPEREALITY_QUATERNION_INL

namespace math
{
	constexpr Quaternion Quaternion::identity = Quaternion{0, 0, 0, 1};

	constexpr Quaternion Quaternion::fromEuler(float x, float y, float z)
	{
		return Quaternion::identity;
	}


}

#endif //SHAPEREALITY_QUATERNION_INL