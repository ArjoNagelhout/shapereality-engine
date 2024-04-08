//
// Created by Arjo Nagelhout on 08/04/2024.
//

#include <gtest/gtest.h>

#include <reflection/enum.h>

using namespace reflection;

namespace enum_info_test
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

    TEST(Reflection, EnumInfo)
    {
        TypeInfoRegistry r;

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

        EnumInfoBuilder<TypeInfo::Type>("TypeInfo::Type")
            .case_(TypeInfo::Type::Class, "Class")
            .case_(TypeInfo::Type::Enum, "Enum")
            .case_(TypeInfo::Type::Primitive, "Primitive")
            .emplace(r);

        std::vector<Wee> items{
            Wee::First,
            Wee::Time,
            Wee::Ive,
            Wee::Ever,
            Wee::Seen,
            Wee::This,
            Wee::Many
        };

        std::cout << "\nWee: \n";
        for (auto& a: r.get<Wee>()->enum_())
        {
            std::cout << a.second << " = " << a.first << std::endl;
        }

        std::cout << "\nTypeInfo::Type: \n";
        for (auto& a: r.get<TypeInfo::Type>()->enum_())
        {
            std::cout << a.second << " = " << a.first << std::endl;
        }

        std::cout << "\nSomething: \n";
        for (auto& a: r.get<Something>()->enum_())
        {
            std::cout << a.second << " = " << a.first << std::endl;
        };
    }
}