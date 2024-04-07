//
// Created by Arjo Nagelhout on 07/04/2024.
//

#include <gtest/gtest.h>

namespace polymorphism_3
{
    struct Class1
    {

    };

    struct Class1_1 : public Class1
    {

    };

    struct Class1_2 : public Class1
    {

    };

    struct Class1_1_1 : public Class1_1
    {

    };

    struct Class1_1_2 : public Class1_1
    {

    };

    struct Class1_2_1 : public Class1_2
    {

    };

    struct Class1_2_2 : public Class1_2
    {

    };

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

    TEST(Polymorphism3, Test)
    {

    }
}