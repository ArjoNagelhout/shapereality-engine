//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_VECTOR_H
#define BORED_ENGINE_VECTOR_H

namespace math
{
	class Vector2
	{
	public:
		explicit Vector2(float x, float y);
		~Vector2();

		float x;
		float y;
	};

	class Vector3
	{
	public:
		explicit Vector3(float x, float y, float z);
		~Vector3();

		float x;
		float y;
		float z;

		static Vector3 up;
		static Vector3 down;
		static Vector3 right;
		static Vector3 left;
		static Vector3 forward;
		static Vector3 back;
		static Vector3 zero;
		static Vector3 one;
	};

	class Vector4
	{
	public:
		explicit Vector4(float x, float y, float z, float w);
		~Vector4();

		float x;
		float y;
		float z;
		float w;
	};
}

#endif //BORED_ENGINE_VECTOR_H
