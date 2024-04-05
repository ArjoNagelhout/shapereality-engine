//
// Created by Arjo Nagelhout on 05/04/2024.
//

#include <gtest/gtest.h>

#include <reflection/type_info.h>

using namespace reflection;

namespace polymorphism_2
{
    struct Base
    {
        virtual ~Base() = default;
    };

    struct Child : public Base
    {
        virtual ~Child() = default;
    };

    struct Child2 : public Child
    {
        virtual ~Child2() = default;
    };

    struct Child3 final : public Child2
    {
        virtual ~Child3() = default;
    };

    struct Container1
    {
        std::vector<std::unique_ptr<Base>> items; // either Base, Child, Child2 or Child3
    };

    struct Container2
    {
        std::unique_ptr<Child2> someItem; // either Child2 or Child3
    };

    struct Container3
    {
        std::unordered_map<std::string, std::unique_ptr<Child>> items; // either Child, Child2 or Child3
    };

    TEST(Reflection, Polymorphism2)
    {
        TypeInfoRegistry r;

        TypeInfoBuilder<Base>("Base")
            .emplace(r);

        TypeInfoBuilder<Child>("Child")
            .base<Base>()
            .emplace(r);

        TypeInfoBuilder<Child2>("Child2")
            .base<Child>()
            .emplace(r);

        TypeInfoBuilder<Child3>("Child3")
            .base<Child2>()
            .emplace(r);

        TypeInfoBuilder<Container1>("Container1")
            .property<&Container1::items>("items")
            .emplace(r);

        TypeInfoBuilder<Container2>("Container2")
            .property<&Container2::someItem>("someItem")
            .emplace(r);

        TypeInfoBuilder<Container3>("Container3")
            .property<&Container3::items>("items")
            .emplace(r);

        std::cout << "polymorphism" << std::endl;
    }
}