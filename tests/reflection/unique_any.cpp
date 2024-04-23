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

    struct Another
    {
        void someLa()
        {

        }

        void operator()()
        {
            std::cout << "operator() called" << std::endl;
        }
    };

    TEST(Reflection, UniqueAny)
    {
        reflection::UniqueAnyPointer a = reflection::makeUniqueAny<SomeType>(10.0f, 5.0, true);

        auto* b = a.get<SomeType>();
        std::cout << "a: " << b->a << std::endl;

        void* c = a.release();
        reflection::AnyDeleter deleter = a.releaseDeleter();
        std::cout << (deleter.valid() ? "true" : "false") << std::endl;

        deleter.operator()(c);

        //std::unique_ptr<SomeType> data = std::make_unique<SomeType>(10.0f, 5.0, true);
    }
}