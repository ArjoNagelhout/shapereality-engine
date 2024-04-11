//
// Created by Arjo Nagelhout on 11/04/2024.
//

#include <gtest/gtest.h>

namespace char_test_dingetjes
{
    TEST(LA, Laa)
    {
        char const* test = "lalalalal";

        char const* ptr = test;
        while (*ptr != '\0') {

            ptr++;
        }
    }
}