//
// Created by Arjo Nagelhout on 30/11/2023.
//

#include <iostream>

#include "../../src/math/vector.h"
#include "../../src/math/vector.inl"

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

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}