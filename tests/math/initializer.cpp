//
// Created by Arjo Nagelhout on 24/04/2024.
//

#include <gtest/gtest.h>

namespace initializer
{
    template<size_t Size, typename Type>
    class Vector
    {
    public:
        Vector(std::initializer_list<Type> items)
        {
            assert(items.size() == Size && "initializer list should be same size as Vector");
            std::copy_n(items.begin(), Size, data.begin());
        }

        Type& operator[](size_t index)
        {
            assert(index < Size && "Index out of range");
            return data[index];
        }

    private:
        std::array<Type, Size> data;
    };

    using Vector3 = Vector<3, float>;

    TEST(Initializer, Vector)
    {
        Vector3 a = Vector3{1, 2, 3};

        std::cout << a[2] << std::endl;
    }
}