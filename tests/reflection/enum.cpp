//
// Created by Arjo Nagelhout on 24/02/2024.
//

#include <gtest/gtest.h>

#include "reflection/enum.h"

using namespace reflection;

namespace enum_test
{
    enum class Something : size_t
    {
        None = 0,
        Yes = 1,
        Something = 2,
        Another = 10,
        Thing = 134
    };

    TEST(Reflection, Enum)
    {
        auto reflected = Enum<Something>()
            .add(Something::None, "None")
            .add(Something::Yes, "Yes")
            .add(Something::Something, "Something")
            .add(Something::Another, "Another")
            .add(Something::Thing, "Thing")
            .build();

        Something a = Something::Something;

        std::cout << "string: " << reflected.toString(a) << std::endl;

        std::cout << "value: " << static_cast<size_t>(reflected.fromString("Thing")) << std::endl;
    }
}