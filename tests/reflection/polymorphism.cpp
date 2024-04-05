//
// Created by Arjo Nagelhout on 05/04/2024.
//

#include <gtest/gtest.h>

namespace polymorphism_test
{
    struct Base
    {
        virtual ~Base() = default;
    };

    struct Child1 : public Base
    {

    };

    struct Child2 : public Base
    {

    };

    using TypeId = size_t;

    struct TypeIndex
    {
        [[nodiscard]] static TypeId getNextId()
        {
            static TypeId value = 1;
            return value++; // first return, then increment
        }
    };

    constexpr TypeId nullType = 0;

    // should be wrapped inside struct to not have internal linkage
    template<typename Type>
    [[nodiscard]] TypeId id()
    {
        static const TypeId value = TypeIndex::getNextId();
        return value;
    }

    struct TypeInfo
    {
        std::string name;
        TypeId base{nullType};
        std::vector<TypeId> children{};

        bool (* isType)(std::any a);
    };

    template<typename Type>
    bool isType(std::any value) noexcept
    {
        return std::any_cast<Type*>(value);
    }

    TEST(Polymorphism, Test)
    {
        std::unordered_map<TypeId, TypeInfo> types{
            {
                id<Base>(),
                TypeInfo{
                    .name = "Base",
                    .children = {
                        id<Child1>(),
                        id<Child2>()
                    }
                }
            },
            {
                id<Child1>(),
                TypeInfo{
                    .name = "Child1",
                    .base = id<Base>(),
                    .children = {

                    }
                }
            },
            {
                id<Child2>(),
                TypeInfo{
                    .name = "Child2",
                    .base = id<Base>(),
                    .children = {

                    }
                }
            }
        };
        std::vector<std::unique_ptr<Base>> items;
        items.emplace_back(std::make_unique<Child2>());
        items.emplace_back(std::make_unique<Child1>());
        items.emplace_back(std::make_unique<Base>());
        items.emplace_back(std::make_unique<Base>());
        items.emplace_back(std::make_unique<Child2>());

        for (int i = 0; i < items.size(); i++)
        {
            std::unique_ptr<Base>& item = items[i];

            // dynamic_cast to a pointer type returns nullptr if not valid

            std::any a = item.get();

            if (dynamic_cast<Child1*>(item.get()))
            {
                std::cout << "is child 1" << std::endl;
            }
            else if (dynamic_cast<Child2*>(item.get()))
            {
                std::cout << "is child 2" << std::endl;
            }
            else
            {
                std::cout << "is base" << std::endl;
            }
            std::cout << "item" << item << std::endl;
        }
    }
}