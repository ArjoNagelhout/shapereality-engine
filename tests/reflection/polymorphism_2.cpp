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

        std::cout << "polymorphism" << std::endl;
    }
}