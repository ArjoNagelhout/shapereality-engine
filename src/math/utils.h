//
// Created by Arjo Nagelhout on 17/12/2023.
//

#ifndef BORED_ENGINE_UTILS_H
#define BORED_ENGINE_UTILS_H

namespace math
{
	constexpr float PI = 3.14159265359f;

	float degreesToRadians(float degrees)
	{
		return degrees * (PI / 180.0f);
	}

	float radiansToDegrees(float radians)
	{
		return radians * 180.0f / PI;
	}
}

#endif //BORED_ENGINE_UTILS_H
