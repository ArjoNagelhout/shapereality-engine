//
// Created by Arjo Nagelhout on 30/11/2023.
//

#include "math/vector.h"
#include "math/vector.inl"

#include "gtest/gtest.h"

TEST(Vector, StaticCasting)
{
    math::Vector2 a = math::Vector2{1, 2};
    auto a_4 = static_cast<math::Vector4>(a);
    auto a_rhs = math::Vector4{1, 2, 0, 0};
    EXPECT_EQ(a_rhs, a_4);
}

TEST(Vector, CrossProduct)
{
    math::Vector3 a = math::Vector3{{5, 6, 7}};
    math::Vector3 b = math::Vector3{{-10, 11, 52}};

    auto result = math::Vector3::cross(a, b);
    math::Vector3 expectedResult = math::Vector3{{235, -330, 115}};

    EXPECT_EQ(result, expectedResult);
}

TEST(Vector, DotProduct)
{
    math::Vector3 a = math::Vector3{{5, 6, 7}};
    math::Vector3 b = math::Vector3{{-10, 11, 52}};

    auto result = math::Vector3::dot(a, b);
    float expectedResult = 380.f;

    EXPECT_EQ(result, expectedResult);
}

TEST(Vector, Clamp)
{
    math::Vector3 vector = math::Vector3{{-13, 30, 23}};
    math::Vector3 min = math::Vector3{{-10, 6, 7}};
    math::Vector3 max = math::Vector3{{10, 11, 52}};

    auto result = math::Vector3::clamp(vector, min, max);
    math::Vector3 expectedResult = math::Vector3{{-10, 11, 23}};

    EXPECT_EQ(result, expectedResult);
}

TEST(Vector, UnitVectors)
{
    math::Vector3 upExpected = math::Vector3{{0, 1, 0}};
    math::Vector3 downExpected = math::Vector3{{0, -1, 0}};
    math::Vector3 leftExpected = math::Vector3{{-1, 0, 0}};
    math::Vector3 rightExpected = math::Vector3{{1, 0, 0}};
    math::Vector3 forwardExpected = math::Vector3{{0, 0, 1}};
    math::Vector3 backExpected = math::Vector3{{0, 0, -1}};

    math::Vector3 up = math::Vector3::up;
    math::Vector3 down = math::Vector3::down;
    math::Vector3 left = math::Vector3::left;
    math::Vector3 right = math::Vector3::right;
    math::Vector3 forward = math::Vector3::forward;
    math::Vector3 back = math::Vector3::back;

    EXPECT_EQ(up, upExpected);
    EXPECT_EQ(down, downExpected);
    EXPECT_EQ(left, leftExpected);
    EXPECT_EQ(right, rightExpected);
    EXPECT_EQ(forward, forwardExpected);
    EXPECT_EQ(back, backExpected);
}