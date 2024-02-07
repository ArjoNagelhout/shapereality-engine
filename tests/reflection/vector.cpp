//
// Created by Arjo Nagelhout on 07/02/2024.
//

#include <gtest/gtest.h>

#include <vector>
#include <any>

#include <reflection/type_id.h>

using namespace reflection;

namespace vector
{
    struct Vector
    {
        std::vector<std::any> data;

        template<typename Type>
        [[nodiscard]] Type& operator[](size_t index)
        {
            return std::any_cast<Type&>(data[index]);
        }

        type_id typeId;
    };

    template<typename Type>
    Vector makeVector()
    {
        Vector v{};
        v.typeId = TypeIndex<Type>::value();
        return v;
    }

    using Something = bool;

    typedef bool SomethingElse;

    TEST(Reflection, Vector)
    {
        static_assert(std::is_same_v<SomethingElse, bool>);
        static_assert(std::is_same_v<Something, bool>);
        Vector things = makeVector<bool>();
        for (int i = 0; i < 100; i++)
        {
            things.data.emplace_back(std::make_any<bool>(i % 2));
        }

        std::cout << "weee" << std::endl;
        bool& a = std::any_cast<bool&>(things.data[0]);
        a = false;
    }
}