//
// Created by Arjo Nagelhout on 20/04/2024.
//

#include <gtest/gtest.h>

#include <reflection/unique_any_pointer.h>

namespace unique_any_tests
{
    struct SomeType
    {
        explicit SomeType(float a_, double b_, bool c_)
            : a(a_), b(b_), c(c_)
        {

        }

        ~SomeType()
        {
            std::cout << "destructor called" << std::endl;
        }

        float a;
        double b;
        bool c;
    };

    TEST(UniqueAnyPointer, MakeUniqueAny)
    {
        reflection::UniqueAnyPointer a = reflection::makeUniqueAny<SomeType>(10.0f, 5.0, true);

        ASSERT_TRUE(a.isType<SomeType>());
        ASSERT_FLOAT_EQ(a.get<SomeType>()->a, 10.0f);
        ASSERT_DOUBLE_EQ(a.get<SomeType>()->b, 5.0);
        ASSERT_EQ(a.get<SomeType>()->c, true);
    }

    struct CustomDeleter
    {
        void operator()(SomeType* someType)
        {
            std::cout << "custom deleter called" << std::endl;
            delete someType;
        }
    };

    TEST(UniqueAnyPointer, FromUniquePointer)
    {
        std::unique_ptr<SomeType> a = std::make_unique<SomeType>(10.0f, 5.0, true);
        reflection::UniqueAnyPointer b = reflection::UniqueAnyPointer(std::move(a));

        ASSERT_EQ(a.get(), nullptr);
        ASSERT_NE(b.get(), nullptr);

        auto* c = new SomeType(10.0f, 5.0, true);
        std::unique_ptr<SomeType, CustomDeleter> d = std::unique_ptr<SomeType, CustomDeleter>(c);

        reflection::UniqueAnyPointer e{};
        ASSERT_TRUE(e.empty());
        e = std::move(d);
        ASSERT_FALSE(e.empty());
    }
}