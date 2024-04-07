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

    struct Child3 : public Child2
    {
        virtual ~Child3() = default;
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

        TypeInfoBuilder<Child3_1>("Child3_1")
            .base<Child3>()
            .emplace(r);

        TypeInfoBuilder<Child3_2>("Child3_2")
            .base<Child3>()
            .emplace(r);

//        TypeInfoBuilder<Container1>("Container1")
//            .property<&Container1::items>("items")
//            .emplace(r);

        TypeInfoBuilder<Container2>("Container2")
            .property<&Container2::someItem>("someItem")
            .emplace(r);

        TypeInfoBuilder<Container3>("Container3")
            .property<&Container3::items>("items")
            .emplace(r);

        Container2 c;
        c.someItem = std::make_unique<Child3_2>();

        TypeId id = r.getChildType(c.someItem.get());
        TypeInfo* info = r.get(id);
        std::cout << "child type is " << info->name << std::endl;

        Container1 c1;
        c1.items.emplace_back(std::make_unique<Base>());
        c1.items.emplace_back(std::make_unique<Child>());
        c1.items.emplace_back(std::make_unique<Child2>());
        c1.items.emplace_back(std::make_unique<Child3>());
        c1.items.emplace_back(std::make_unique<Child3_1>());
        c1.items.emplace_back(std::make_unique<Child3_2>());

        for (auto& item: c1.items)
        {
            TypeId itemId = r.getChildType(item.get());
            TypeInfo* itemInfo = r.get(itemId);
            std::cout << "item type is: " << itemInfo->name << std::endl;
        }
    }

    // it's hard to implement for the general case because we need
    // to pass the BaseType as a compile time argument, even though we
    // only know it at runtime.
    // so it would be easy to implement for just one level of polymorphism,
    // but that is not elegant

    // we only know the type hierarchy at runtime
    // not at compile time, however we need to do the any_cast
    // to the base type at compile time
    // that is why I'm not able to implement this.
    //
    // attempting any_cast directly to the child type
    // will result in a bad_any_cast as it requires the created std::any
    // pointer to be of the child type, and we store it as a pointer to the base type
    //
    // how do we marry compile time and run time?
    //
    // how do we register it currently?
    //
    // 1. we call .base(), which adds a child to the base type and
    // sets the base to the provided argument
    //
    // 2. we register a PointerNode, which contains the base type
    // it could be.
    //
    // in order to store isType for each TypeInfo, we need to know the basest type
    // maybe we can cast it up only one step at a time.
    //
    // let's see if I can implement this.
}