//
// Created by Arjo Nagelhout on 02/12/2023.
//

#include <math/quaternion.h>
#include <math/quaternion.inl>

#include "gtest/gtest.h"

namespace quaternion_tests
{
    TEST(Quaternion, FromEulerAngles)
    {
        math::Vector3f eulerAngles{0, 0, 0};

        math::Quaternionf expected = math::Quaternionf::identity;
        math::Quaternionf result = math::Quaternionf::createFromEulerInDegrees(eulerAngles);
        math::Quaternionf result2 = math::Quaternionf::createFromEulerInRadians(eulerAngles);

        ASSERT_EQ(result, expected);
        ASSERT_EQ(result2, expected);
    }

    TEST(Quaternion, Multiplication)
    {
        math::Quaternionf a = math::Quaternionf::identity;
        math::Quaternionf b = math::Quaternionf::identity;

        math::Quaternionf expected = math::Quaternionf::identity;
        math::Quaternionf result = a * b;

        ASSERT_EQ(result, expected);
    }
}


