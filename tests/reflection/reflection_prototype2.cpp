//
// Created by Arjo Nagelhout on 05/02/2024.
//

#include <gtest/gtest.h>

#include <any>

namespace prototype2
{
    struct SimpleData
    {
        float value1;
        int value2;
        bool value3;
    };

    void function(int parameter)
    {
        std::cout << "parameter: " << parameter << std::endl;
    }

    template<typename Type>
    void function2(Type type)
    {
        std::cout << "function2: " << type << std::endl;
    }

    void set(std::any a)
    {

    }

    TEST(Reflection, Prototype2)
    {
        SimpleData d{
            .value1 = 3.0f,
            .value2 = 32,
            .value3 = true,
        };

        float SimpleData::* v1 = &SimpleData::value1;

        ASSERT_EQ(d.value1, 3.0f);
        d.*v1 = 12.0f;
        ASSERT_EQ(d.value1, 12.0f);

        function(32);
        void (* a)(int) = &function; // function pointer
        a(16);

        function2(32);
        function2("something");
        void (* f2_int)(int) = &function2;
        void (* f2_string)(std::string) = &function2;

        f2_int(1);
        f2_string("f2_string");

        // let's try out any
        std::any (* f3)() = &function3;


    }
}