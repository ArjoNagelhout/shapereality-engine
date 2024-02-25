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

    enum class Wee : unsigned int
    {
        First = 1000,
        Time = 1200,
        Ive = 1300,
        Ever = 1400,
        Seen = 1500,
        This = 1230,
        Many = 6000
    };

    TEST(Reflection, Enum)
    {
        auto reflected = Enum()
            .add(Something::None, "None")
            .add(Something::Yes, "Yes")
            .add(Something::Something, "Something")
            .add(Something::Another, "Another")
            .add(Something::Thing, "Thing");

        auto d = Enum()
            .add(Wee::First, "First")
            .add(Wee::Time, "Time")
            .add(Wee::Ive, "Ive")
            .add(Wee::Ever, "Ever")
            .add(Wee::Seen, "Seen")
            .add(Wee::This, "This")
            .add(Wee::Many, "Many");

        EnumSerializer s;
        s.emplace<Something>(std::move(reflected));
        s.emplace<Wee>(std::move(d));

        Something a = Something::Something;

        std::cout << "string: " << s.toString(a) << std::endl;

        std::cout << "value: " << static_cast<size_t>(s.fromString<Something>("Thing")) << std::endl;
    }
}