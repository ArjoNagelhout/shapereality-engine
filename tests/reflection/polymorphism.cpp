//
// Created by Arjo Nagelhout on 05/04/2024.
//

#include <gtest/gtest.h>

#include <reflection/class.h>
#include <reflection/reflection.h>

using namespace reflection;

namespace polymorphism
{
    struct Base
    {
        virtual ~Base() = default;
    };

    struct Child : public Base
    {
    };

    struct Child2 : public Child
    {
    };

    struct Child3 : public Child2
    {
    };

    struct Child3_1 : public Child3
    {
    };

    struct Child3_2 : public Child3
    {
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

    TEST(Reflection, Polymorphism)
    {
        Reflection& reflection = Reflection::shared();
        TypeInfoRegistry& r = reflection.types;

        ClassInfoBuilder<Base>("Base")
            .emplace(r);

        ClassInfoBuilder<Child>("Child")
            .base<Base>()
            .emplace(r);

        ClassInfoBuilder<Child2>("Child2")
            .base<Child>()
            .emplace(r);

        ClassInfoBuilder<Child3>("Child3")
            .base<Child2>()
            .emplace(r);

        ClassInfoBuilder<Child3_1>("Child3_1")
            .base<Child3>()
            .emplace(r);

        ClassInfoBuilder<Child3_2>("Child3_2")
            .base<Child3>()
            .emplace(r);

        ClassInfoBuilder<Container1>("Container1")
            .member<&Container1::items>("items")
            .emplace(r);

        ClassInfoBuilder<Container2>("Container2")
            .member<&Container2::someItem>("someItem")
            .emplace(r);

        ClassInfoBuilder<Container3>("Container3")
            .member<&Container3::items>("items")
            .emplace(r);

        Container2 c;
        c.someItem = std::make_unique<Child3_2>();

        TypeId id = r.getChildType(c.someItem.get());
        ASSERT_EQ(id, TypeIndex<Child3_2>::value());
        TypeInfo* info = r.get(id);

        std::cout << "child type is " << info->name << std::endl;

        Container1 c1;
        c1.items.emplace_back(std::make_unique<Base>());
        c1.items.emplace_back(std::make_unique<Child>());
        c1.items.emplace_back(std::make_unique<Child2>());
        c1.items.emplace_back(std::make_unique<Child3>());
        c1.items.emplace_back(std::make_unique<Child3_1>());
        c1.items.emplace_back(std::make_unique<Child3_2>());
        c1.items.emplace_back(std::make_unique<Child>());

        std::vector<TypeId> targets{
            TypeIndex<Base>::value(),
            TypeIndex<Child>::value(),
            TypeIndex<Child2>::value(),
            TypeIndex<Child3>::value(),
            TypeIndex<Child3_1>::value(),
            TypeIndex<Child3_2>::value(),
            TypeIndex<Child>::value()
        };

        for (size_t i = 0; i < c1.items.size(); i++)
        {
            Base* item = c1.items[i].get();
            TypeId target = targets[i];

            TypeId itemId = r.getChildType(item);
            ASSERT_EQ(itemId, target);

            TypeInfo* itemInfo = r.get(itemId);
            std::cout << "item type is: " << itemInfo->name << std::endl;
        }
    }
}