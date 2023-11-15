//
// Created by Arjo Nagelhout on 16/11/2023.
//

#ifndef BORED_ENGINE_QUATERNION_H
#define BORED_ENGINE_QUATERNION_H

namespace math
{
	class Quaternion
	{
	public:
		explicit Quaternion(float x, float y, float z, float w);
		~Quaternion();

		float x;
		float y;
		float z;
		float w;
	};
}

#endif //BORED_ENGINE_QUATERNION_H
