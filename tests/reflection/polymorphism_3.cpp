//
// Created by Arjo Nagelhout on 07/04/2024.
//

#include <gtest/gtest.h>

#include <utility>

namespace polymorphism_3
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

    struct Child1_1 : public Child1
    {

    };

    struct Child1_2 : public Child1
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
        TypeId base = nullType;
        std::vector<TypeId> children{};

        bool (* isType)(std::any);

        std::any (* castDownToThisType)(std::any);
    };

    template<typename Type, typename BaseType>
    bool isType(std::any value)
    {
        auto** v = std::any_cast<BaseType*>(&value);
        assert(v);
        return dynamic_cast<Type*>(*v);
    }

    // should only be called if we know the downcast is possible
    template<typename Type, typename BaseType>
    std::any castDownToThisType(std::any value)
    {
        // don't need to downcast
        if (std::any_cast<Type*>(&value))
        {
            return value;
        }

        // otherwise, downcast
        auto** v = std::any_cast<BaseType*>(&value);
        assert(v);
        return dynamic_cast<Type*>(*v);
    }

    // dynamic cast requires the input value to be of the base type, because
    // otherwise it doesn't have a reference to the vtable, and thus no information
    // about the type hierarchy.
    //
    // The issue is that only the PointerNode knows which base type exists, but it can only store
    // a generic function that takes std::any as arguments or has std::any as return value
    //
    // the std::any cast is what should be done by PointerNode

    // pointer node knows to which base node we need to cast
    // so the function pointer it contains should provide the conversion
    // to the base type
    // however, we can only do this generically

    struct TypeInfoRegistry
    {
        std::unordered_map<TypeId, TypeInfo> types{
            {
                id<Base>(),
                TypeInfo{
                    .name = "Base",
                    .base = nullType,
                    .children = {
                        id<Child1>(),
                        id<Child2>()
                    },
                    .isType = isType<Base, Base>,
                    .castDownToThisType = castDownToThisType<Base, Base>
                }
            },
            {
                id<Child1>(),
                TypeInfo{
                    .name = "Child1",
                    .base = id<Base>(),
                    .children = {
                        id<Child1_1>(),
                        id<Child1_2>()
                    },
                    .isType = isType<Child1, Base>,
                    .castDownToThisType = castDownToThisType<Child1, Base>
                }
            },
            {
                id<Child2>(),
                TypeInfo{
                    .name = "Child2",
                    .base = id<Base>(),
                    .children = {

                    },
                    .isType = isType<Child2, Base>,
                    .castDownToThisType = castDownToThisType<Child2, Base>
                }
            },
            {
                id<Child1_1>(),
                TypeInfo{
                    .name = "Child1_1",
                    .base = id<Child1>(),
                    .children = {

                    },
                    .isType = isType<Child1_1, Child1>,
                    .castDownToThisType = castDownToThisType<Child1_1, Child1>
                }
            },
            {
                id<Child1_2>(),
                TypeInfo{
                    .name = "Child1_2",
                    .base = id<Child1>(),
                    .children = {

                    },
                    .isType = isType<Child1_2, Child1>,
                    .castDownToThisType = castDownToThisType<Child1_2, Child1>
                }
            }
        };

        // get polymorphic type (subclass)
        // std::any value = pointer to value
        // id = base type
        [[nodiscard]] TypeId getChildType(std::any value, TypeId id)
        {
            if (!types.contains(id))
            {
                return nullType;
            }

            TypeInfo& base = types.at(id);

            std::any thisValue = base.castDownToThisType(std::move(value));

            for (auto& childTypeId: base.children)
            {
                // for each child, check if it is that type, and if so, recurse
                TypeInfo& child = types.at(childTypeId);
                if (child.isType(thisValue))
                {
                    return getChildType(thisValue, childTypeId);
                }
            }

            // none of the children's isType (dynamic_cast) succeeded
            return id;
        }

        template<typename Type>
        [[nodiscard]] TypeId getChildType(Type* value)
        {
            return getChildType(value, id<Type>());
        }
    };

    TEST(Polymorphism, Test3)
    {
        TypeInfoRegistry r;
        std::vector<std::unique_ptr<Base>> items;
        items.emplace_back(std::make_unique<Child2>());
        items.emplace_back(std::make_unique<Child1>());
        items.emplace_back(std::make_unique<Base>());
        items.emplace_back(std::make_unique<Base>());
        items.emplace_back(std::make_unique<Child2>());
        items.emplace_back(std::make_unique<Child1_1>());
        items.emplace_back(std::make_unique<Child1_2>());

        for (int i = 0; i < items.size(); i++)
        {
            std::unique_ptr<Base>& item = items[i];

            TypeId id = r.getChildType<Base>(item.get());
            std::cout << "getChildType returned " << r.types[id].name << std::endl;
        }

        std::vector<std::unique_ptr<Child1>> items2;
        items2.emplace_back(std::make_unique<Child1>());
        items2.emplace_back(std::make_unique<Child1_1>());
        items2.emplace_back(std::make_unique<Child1_2>());

        for (int i = 0; i < items2.size(); i++)
        {
            std::unique_ptr<Child1>& item = items2[i];

            TypeId id = r.getChildType<Child1>(item.get());
            std::cout << "getChildType for items2 returned " << r.types[id].name << std::endl;
        }
    }
}