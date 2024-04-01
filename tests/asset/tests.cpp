//
// Created by Arjo Nagelhout on 28/03/2024.
//

#include <gtest/gtest.h>

namespace lalala
{
    struct Base
    {
        void test(bool a, bool b, bool c = false)
        {
            testImpl(a, b, c);
        }
        virtual void testImpl(bool a, bool b, bool c) = 0;
    };

    struct Child : public Base
    {
        void test(bool a, bool b, bool c = false) override
        {

        }
    };

    TEST(Lalala, A)
    {
        Child a;
        a.test(true, true);
    }
}