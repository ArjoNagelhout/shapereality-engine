//
// Created by Arjo Nagelhout on 09/02/2024.
//

#include <gtest/gtest.h>
#include <reflection/type_info.h>

#include <vector>

namespace vector_prototype2
{
    // we want to serialize and deserialize vectors.
    // a vector should not have to be separately registered, just as a property
    // registering the property gives us the type std::vector<certain type>, so
    // can write some template that registers that it is a vector<>
    // let's first test if we can detect it being a vector

    struct Complex
    {
        bool a = true;
        bool b = false;
        bool c = true;
        bool d = false;
    };

    struct Data
    {
        std::vector<bool> a;
        std::vector<Complex> b;
    };

    // full template specialization:
    template<typename Type>
    struct Something
    {

    };

    template<>
    struct Something<int>
    {

    };

    // sfinae
    template<typename Type>
    void f(typename Type::subtype t)
    {
        std::cout << "subtype" << std::endl;
    }

    template<typename Type>
    void f(Type t)
    {
        std::cout << "no subtype" << std::endl;
    }

    struct Valid
    {
        using subtype = bool;
    };

    struct Invalid
    {

    };

    TEST(Reflection, VectorPrototype2_1)
    {
        Valid v;
        f<Valid>(v);
        Invalid a;
        f<Invalid>(a);
    }

    // detect vector

    template<typename T>
    T test(T a, T b)
    {
        return b < a ? a : b;
    }

    TEST(Reflection, VectorPrototype2_2)
    {

    }

    TEST(Reflection, VectorPrototype2)
    {
        Data d{
            .a{
                {false, false, true, false, true}
            },
            .b{
                {
                    .a = false,
                    .b = true,
                    .c = false,
                    .d = true
                },
                {
                    .a = true,
                    .b = true,
                    .c = false,
                    .d = false
                },
                {
                    .a = false,
                    .b = false,
                    .c = false,
                    .d = true
                },
            }
        };


    }
}