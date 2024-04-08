//
// Created by Arjo Nagelhout on 24/02/2024.
//

#include <gtest/gtest.h>

#include <reflection/serialize/enum.h>
#include <reflection/serialize/json.h>

#include <reflection/class.h>
#include <reflection/enum.h>

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

    struct Data
    {
        Wee lala = Wee::Seen;
        Something soso = Something::Thing;
    };

    TEST(Reflection, Enum)
    {
        TypeInfoRegistry r;
        EnumSerializer enums;
        JsonSerializer serializer(r, enums);

        ClassInfoBuilder<Data>("Data")
            .member<&Data::lala>("lala")
            .member<&Data::soso>("soso")
            .emplace(r);

        r.emplace<Something>(std::make_unique<EnumInfo>("Something"));
        EnumBuilder<Something>()
            .add(Something::None, "None")
            .add(Something::Yes, "Yes")
            .add(Something::Something, "Something")
            .add(Something::Another, "Another")
            .add(Something::Thing, "Thing")
            .emplace(enums);

        r.emplace<Wee>(std::make_unique<EnumInfo>("Wee"));
        EnumBuilder<Wee>()
            .add(Wee::First, "First")
            .add(Wee::Time, "Time")
            .add(Wee::Ive, "Ive")
            .add(Wee::Ever, "Ever")
            .add(Wee::Seen, "Seen")
            .add(Wee::This, "This")
            .add(Wee::Many, "Many")
            .emplace(enums);

        Data data{
            .lala = Wee::Time,
            .soso = Something::Something
        };

        std::cout << serializer.toJsonString(data, 2) << std::endl;

        Data newData = serializer.fromJsonString<Data>(R"({
  "lala": "Time",
  "soso": "Something"
})");

        Something a = Something::Something;

        std::cout << "string: " << enums.toString(a) << std::endl;

        std::cout << "value: " << static_cast<size_t>(enums.fromString<Something>("Thing")) << std::endl;
    }
}