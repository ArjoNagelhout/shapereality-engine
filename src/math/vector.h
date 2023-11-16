//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_VECTOR_H
#define BORED_ENGINE_VECTOR_H

#include <string>

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

	class Vector2Int
	{
	public:
		explicit Vector2Int(int x, int y);
		~Vector2Int();

		int x;
		int y;
	};

	class Vector3
	{
	public:
		explicit Vector3(float x, float y, float z);
		~Vector3();

		// static properties

		static Vector3 back;
		static Vector3 down;
		static Vector3 forward;
		static Vector3 left;
		static Vector3 negativeInfinity;
		static Vector3 one;
		static Vector3 positiveInfinity;
		static Vector3 right;
		static Vector3 up;
		static Vector3 zero;

		// properties
		float x;
		float y;
		float z;

		float getMagnitude();
		Vector3 getNormalized();
		float getSqrMagnitude();
		float operator[](int const& index); // 0 = x, 1 = y, 2 = z

		// public methods
		void set(float const& x, float const& y, float const& z);
		std::string toString();

		// static methods
		static float angle(Vector3 const& from, Vector3 const& to);
		static Vector3 clampMagnitude(Vector3 const& vector, float const& maxLength);
		static Vector3 cross(Vector3 const& lhs, Vector3 const& rhs);
		static float distance(Vector3 const& a, Vector3 const& b);
		static float dot(Vector3 const& a, Vector3 const& b);
		static Vector3 lerp(Vector3 const& a, Vector3 const& b, float const& t);
		static Vector3 lerpUnclamped(Vector3 const& a, Vector3 const& b, float const& t);
		static Vector3 max(Vector3 const& lhs, Vector3 const& rhs);
		static Vector3 min(Vector3 const& lhs, Vector3 const& rhs);
		static Vector3 moveTowards(Vector3 const& current, Vector3 const& target, float const& maxDistanceDelta);
		static Vector3 normalize(Vector3 const& value);
		static Vector3 orthoNormalize(Vector3 const& normal, Vector3 const& tangent);
		static Vector3 project(Vector3 const& vector, Vector3 const& onNormal);
		static Vector3 projectOnPlane(Vector3 const& vector, Vector3 const& planeNormal);
		static Vector3 reflect(Vector3 const& inDirection, Vector3 const& inNormal);
		static Vector3 rotateTowards(Vector3 const& current, Vector3 const& target, float const& maxRadiansDelta, float const& maxMagnitudeDelta);
		static Vector3 scale(Vector3 const& a, Vector3 const& b);
		static float signedAngle(Vector3 const& from, Vector3 const& to, Vector3 const& axis);
		static Vector3 slerp(Vector3 const& a, Vector3 const& b, float const& t);
		static Vector3 slerpUnclamped(Vector3 const& a, Vector3 const& b, float const& t);
		static Vector3 smoothDamp(Vector3 const& current, Vector3 const& target, Vector3 const& currentVelocity, float const& smoothTime, float const& maxSpeed, float const& deltaTime);

		// operators
		Vector3 operator-(Vector3 const& v);
		Vector3 operator*(float const& d);
		Vector3 operator/(float const& d);
		Vector3 operator+(Vector3 const& v);
	};

	class Vector3Int
	{
	public:
		explicit Vector3Int(int x, int y, int z);
		~Vector3Int();

		int x;
		int y;
		int z;
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
