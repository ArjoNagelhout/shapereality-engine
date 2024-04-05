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
    bool isType(std::any value)
    {
        Type** v = std::any_cast<Type>(&value);
        return v != nullptr && *v != nullptr;
    }

    struct TypeInfoRegistry
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

        // get polymorphic type (subclass)
        // std::any value = pointer to value
        // id = base type
        [[nodiscard]] TypeId getSubType(std::any value, TypeId id)
        {
            if (!types.contains(id))
            {
                return nullType;
            }

            TypeInfo& base = types.at(id);

            for (auto& childTypeId: base.children)
            {
                // for each child, check if it is that type, and if so, recurse
                TypeInfo& child = types.at(childTypeId);
                if (child.isType(value))
                {
                    return getSubType(value, childTypeId);
                }
            }

            // none of the children's isType (dynamic_cast) succeeded
            return id;
        }

        template<typename Type>
        [[nodiscard]] TypeId getSubType(std::any value)
        {
            return getSubType(value, id<Type>());
        }
    };

    TEST(Polymorphism, Test)
    {
        TypeInfoRegistry r;
        std::vector<std::unique_ptr<Base>> items;
        items.emplace_back(std::make_unique<Child2>());
        items.emplace_back(std::make_unique<Child1>());
        items.emplace_back(std::make_unique<Base>());
        items.emplace_back(std::make_unique<Base>());
        items.emplace_back(std::make_unique<Child2>());

        for (int i = 0; i < items.size(); i++)
        {
            std::unique_ptr<Base>& item = items[i];
            std::cout << "item " << i << ": " << item << std::endl;

            // dynamic_cast to a pointer type returns nullptr if not valid

            std::any a = item.get();

            Base** aa = std::any_cast<Base*>(&a);
            assert(aa);
            Base* o = *aa;

            if (dynamic_cast<Child1*>(o))
            {
                std::cout << "child1" << std::endl;
            }
            else if (dynamic_cast<Child2*>(o))
            {
                std::cout << "child2" << std::endl;
            }
            else
            {
                std::cout << "base" << std::endl;
            }
        }
    }

    TEST(A, B) {
        std::any a = std::make_any<Child1*>(new Child1); // Storing a pointer to Child1 in std::any

        // Attempt to cast using a pointer to the std::any instance to avoid exceptions
        auto ptr = std::any_cast<Child1*>(&a);
        if (ptr) {
            std::cout << "a contains a pointer to Child1." << std::endl;
        } else {
            std::cout << "a does not contain a pointer to Child1." << std::endl;
        }

        // If ptr is not null, it means the cast was successful, and 'a' did contain a pointer to Child1.
        // Remember to manage memory properly if you dynamically allocate memory for Child1.
        // In this example, we're assuming ownership is elsewhere, so no delete is called here.


    }

    // problem:
    // in order to perform a dynamic_cast we need to first do an any_cast
    // to the base type. However, we're not sure how far down we could do a base
    // cast
    //
    // e.g. we might have:
    // struct Base {}
    // struct Child : public Base {}
    // struct Child2 : public Child {}
    // struct Child3 : public Child2 {}
    //
    // if we then have a
    // std::vector<std::unique_ptr<Child2>>
    // we can't base cast to the top most class
    // as that would throw a bad_any_cast exception
    //
    // however, we might be able to solve this
    // by simply using any_cast, without having
    // to use dynamic_cast, by using the
    // noexcept version of the method.
}