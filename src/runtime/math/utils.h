//
// Created by Arjo Nagelhout on 17/12/2023.
//

#ifndef SHAPEREALITY_UTILS_H
#define SHAPEREALITY_UTILS_H

namespace math
{
	constexpr float PI = 3.14159265359f;

	constexpr float degreesToRadians(float degrees)
	{
		return degrees * (PI / 180.0f);
	}

	constexpr float radiansToDegrees(float radians)
	{
		return radians * 180.0f / PI;
	}
}

#endif //SHAPEREALITY_UTILS_H
