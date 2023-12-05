//
// Created by Arjo Nagelhout on 16/11/2023.
//

#ifndef BORED_ENGINE_QUATERNION_H
#define BORED_ENGINE_QUATERNION_H

namespace math
{
	// can be used to represent rotations
	struct Quaternion final
	{
		constexpr explicit Quaternion(float x, float y, float z, float w)
		{
		}

		constexpr ~Quaternion() = default;

	private:
		float x;
		float y;
		float z;
		float w;
	};
}

#endif //BORED_ENGINE_QUATERNION_H
