//
// Created by Arjo Nagelhout on 24/02/2024.
//

#include <gtest/gtest.h>

#include <reflection/serialize/json.h>

#include <reflection/class.h>
#include <reflection/enum.h>
#include <reflection/reflection.h>

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
        Reflection& reflection = Reflection::shared();
        TypeInfoRegistry& r = reflection.types;
        JsonSerializer& serializer = reflection.json;

        ClassInfoBuilder<Data>("Data")
            .member<&Data::lala>("lala")
            .member<&Data::soso>("soso")
            .emplace(r);

        EnumInfoBuilder<Something>("Something")
            .case_(Something::None, "None")
            .case_(Something::Yes, "Yes")
            .case_(Something::Something, "Something")
            .case_(Something::Another, "Another")
            .case_(Something::Thing, "Thing")
            .emplace(r);

        EnumInfoBuilder<Wee>("Wee")
            .case_(Wee::First, "First")
            .case_(Wee::Time, "Time")
            .case_(Wee::Ive, "Ive")
            .case_(Wee::Ever, "Ever")
            .case_(Wee::Seen, "Seen")
            .case_(Wee::This, "This")
            .case_(Wee::Many, "Many")
            .emplace(r);

        Data data{
            .lala = Wee::Time,
            .soso = Something::Something
        };

        std::cout << serializer.toJsonString(data, 2) << std::endl;

        Data newData = serializer.fromJsonString<Data>(R"({
  "lala": "Time",
  "soso": "Something"
})");
        ASSERT_EQ(data.lala, newData.lala);
        ASSERT_EQ(data.soso, newData.soso);

        std::vector<std::string> items{
            "First",
            "Time",
            "Ive",
            "Ever",
            "Seen",
            "This",
            "Many"
        };

        std::cout << "\nWee: \n";
        int i = 0;
        for (auto& a: r.get<Wee>()->enum_())
        {
            std::cout << a.second << " = " << a.first << std::endl;
            ASSERT_EQ(a.second, items[i]);
            i++;
        }
    }
}