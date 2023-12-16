//
// Created by Arjo Nagelhout on 30/11/2023.
//

#include "math/vector.h"
#include "math/vector.inl"

#include "gtest/gtest.h"

//------------------------------------------
// Vector
//------------------------------------------

TEST(Vector, StaticCasting)
{
	math::vec2 a = math::vec2{{1, 2}};
	auto a_4 = static_cast<math::vec4>(a);
	auto a_rhs = math::vec4{{1, 2, 0, 0}};
	EXPECT_EQ(a_rhs, a_4);
}

TEST(Vector, CrossProduct)
{
	math::vec3 a = math::vec3{{5, 6, 7}};
	math::vec3 b = math::vec3{{-10, 11, 52}};

	auto result = math::vec3::cross(a, b);
	math::vec3 expectedResult = math::vec3{{235, -330, 115}};

	EXPECT_EQ(result, expectedResult);
}

TEST(Vector, DotProduct)
{
	math::vec3 a = math::vec3{{5, 6, 7}};
	math::vec3 b = math::vec3{{-10, 11, 52}};

	auto result = math::vec3::dot(a, b);
	float expectedResult = 380.f;

	EXPECT_EQ(result, expectedResult);
}

TEST(Vector, Clamp)
{
	math::vec3 vector = math::vec3{{-13, 30, 23}};
	math::vec3 min = math::vec3{{-10, 6, 7}};
	math::vec3 max = math::vec3{{10, 11, 52}};

	auto result = math::vec3::clamp(vector, min, max);
	math::vec3 expectedResult = math::vec3{{-10, 11, 23}};

	EXPECT_EQ(result, expectedResult);
}

TEST(Vector, CreateOne)
{
	auto one = math::Vector<6>::one;
	auto expected = math::Vector<6>{{1, 1, 1, 1, 1, 1}};

	EXPECT_EQ(one, expected);
}

TEST(Vector, UnitVectors)
{
	math::vec3 upExpected = math::vec3{{0, 1, 0}};
	math::vec3 downExpected = math::vec3{{0, -1, 0}};
	math::vec3 leftExpected = math::vec3{{-1, 0, 0}};
	math::vec3 rightExpected = math::vec3{{1, 0, 0}};
	math::vec3 forwardExpected = math::vec3{{0, 0, 1}};
	math::vec3 backExpected = math::vec3{{0, 0, -1}};

	math::vec3 up = math::vec3::up;
	math::vec3 down = math::vec3::down;
	math::vec3 left = math::vec3::left;
	math::vec3 right = math::vec3::right;
	math::vec3 forward = math::vec3::forward;
	math::vec3 back = math::vec3::back;

	EXPECT_EQ(up, upExpected);
	EXPECT_EQ(down, downExpected);
	EXPECT_EQ(left, leftExpected);
	EXPECT_EQ(right, rightExpected);
	EXPECT_EQ(forward, forwardExpected);
	EXPECT_EQ(back, backExpected);
}