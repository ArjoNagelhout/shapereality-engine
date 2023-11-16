#include "vector.h"

namespace math
{
	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
	{

	}

	Vector3::~Vector3() = default;

	Vector3 Vector3::back = Vector3(0, 0, -1);
	Vector3 Vector3::down = Vector3(0, -1, 0);
	Vector3 Vector3::forward = Vector3(0, 0, 1);
	Vector3 Vector3::left = Vector3(-1, 0, 0);
	Vector3 Vector3::negativeInfinity = Vector3(-1, -1, -1); // todo
	Vector3 Vector3::one = Vector3(1, 1, 1);
	Vector3 Vector3::positiveInfinity = Vector3(1, 1, 1); // todo
	Vector3 Vector3::right = Vector3(1, 0, 0);
	Vector3 Vector3::up = Vector3(0, 1, 0);
	Vector3 Vector3::zero = Vector3(0, 0, 0);
}