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
        TypeId base = nullType;
        std::vector<TypeId> children{};

        bool (* isType)(std::any);
    };

    template<typename Type, typename BaseType>
    bool isType(std::any value)
    {
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
                    .isType = isType<Base, Base>
                }
            },
            {
                id<Child1>(),
                TypeInfo{
                    .name = "Child1",
                    .base = id<Base>(),
                    .children = {

                    },
                    .isType = isType<Child1, Base>
                }
            },
            {
                id<Child2>(),
                TypeInfo{
                    .name = "Child2",
                    .base = id<Base>(),
                    .children = {

                    },
                    .isType = isType<Child2, Base>
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

            for (auto& childTypeId: base.children)
            {
                // for each child, check if it is that type, and if so, recurse
                TypeInfo& child = types.at(childTypeId);
                if (child.isType(value))
                {
                    return getChildType(value, childTypeId);
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

            TypeId id = r.getChildType<Base>(item.get());
            std::cout << "getChildType returned " << r.types[id].name << std::endl;
        }
    }
    
    // dynamic_cast to a pointer type returns nullptr if not valid

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
    //
    // no, that doesn't work, because any_cast returns false if the pointer that was stored in std::any
    // is of the base type, and we try to do an any_cast to the child type.
    // so we do *need* dynamic_cast. Because that's the only way to perform runtime polymorphism checking.
    //
    // we also *need* any_cast, because dynamic_cast does not work on void*, it only works if we have a pointer
    // to the base type.
    // for each parent type we would need to have a separate is type function, so that we can cast to the base type.
    // this is actually doable with the Pointer type node.
    // so a pointer type node contains an isType function pointer with instantiated template of isType with the correct
    // base type for that specific pointer node.

}