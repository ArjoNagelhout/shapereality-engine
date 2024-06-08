//
// Created by Arjo Nagelhout on 08/04/2024.
//

#ifndef SHAPEREALITY_CLASS_H
#define SHAPEREALITY_CLASS_H

#include <reflection/type_info.h>
#include <reflection/handlers/any_unordered_map_handler.h>
#include <reflection/handlers/any_vector_handler.h>

namespace reflection
{
    // helpers for detecting whether something is a std::vector or std::unordered_map
    template<typename>
    struct IsVector : std::false_type
    {
    };

    template<typename Type>
    struct IsVector<std::vector<Type>> : std::true_type
    {
    };

    template<typename>
    struct IsUnorderedMap : std::false_type
    {
    };

    template<typename Key, typename Value>
    struct IsUnorderedMap<std::unordered_map<Key, Value>> : std::true_type
    {
    };

    template<typename>
    struct IsUniquePointer : std::false_type
    {
    };

    template<typename Type>
    struct IsUniquePointer<std::unique_ptr<Type>> : std::true_type
    {
    };

    struct PropertyInfo final
    {
        std::string name; // name of property
        size_t node; // index to node of property (we don't use pointers so that TypeInfo is trivially copyable)

        std::any (* get)(std::any); // function pointer to get pointer to value of this property
    };

    struct TypePropertyNode final
    {
        TypeId typeId; // type
    };

    struct VectorPropertyNode final
    {
        using Action = any_vector_handler_implementation::Action;

        size_t valueNode; // index to TypeNode, Value of std::vector<Value>

        std::any (* handle)(Action action, std::any this_, std::any const& other);
    };

    struct UnorderedMapPropertyNode final
    {
        using Action = any_unordered_map_handler_implementation::Action;

        TypeId keyTypeId; // Key of std::unordered_map<Key, Value>
        size_t valueNode; // index to TypeNode, mapped type of std::unordered_map

        std::any (* handle)(Action action, std::any this_, std::any const& other);

        [[nodiscard]] std::any insert(std::any this_, std::any value);

        [[nodiscard]] std::any at(std::any this_, std::any const& key);

        void clear();
    };

    struct UniquePointerPropertyNode final
    {
        // we don't aim to support a pointer inside a pointer, or pointer to a dictionary as that would make no sense
        // therefore, we can directly store the type id
        TypeId valueTypeId;
    };

    struct PropertyNode final
    {
        enum class Type
        {
            Type, // a type, either an enum, primitive or class / struct
            Vector, // std::vector
            UnorderedMap, // std::unordered_map
            UniquePtr // std::unique_ptr
        };

        Type type;

        union
        {
            TypePropertyNode object;
            VectorPropertyNode list;
            UnorderedMapPropertyNode dictionary;
            UniquePointerPropertyNode pointer;
        };
    };

    /**
     * A reflected class contains information about:
     * - inheritance
     *     - which child classes inherit from this class,
     *     - the base class of this class
     * - properties
     *
     * Each property has a name and is represented by a node tree.
     * These nodes enable reflection of nested container types, such as
     * std::unordered_map and std::vector:
     *
     * `std::vector<std::vector<float>>` becomes:
     *
     * ```
     * VectorPropertyNode{
     *  .valueNode = VectorPropertyNode{
     *      .valueNode = TypePropertyNode{ .typeId = TypeIndex<float>::value() }
     *  }
     * }
     * ```
     *
     * and std::unordered_map<std::string, std::vector<
     *
     * A TypePropertyNode refers to a TypeInfo inside the TypeRegistry,
     * which enables recursively reflecting types such as:
     *
     * ```
     * struct A
     * {
     *     float b;
     * }
     *
     * struct B
     * {
     *     A a;
     *     A a2;
     *     A a3;
     * }
     * ```
     *
     */
    struct ClassInfo final : public TypeInfo
    {
        explicit ClassInfo(std::string name);

        TypeId base = nullTypeId; // base class
        std::vector<TypeId> children; // child classes
        std::vector<PropertyNode> nodes; // container that owns all nodes that describe this type
        std::vector<PropertyInfo> properties; // list of properties inside this type

        bool (* isType)(std::any) = nullptr; // whether the given base type is of this type (performs a dynamic cast check)

        std::any (* downcast)(std::any) = nullptr; // casts down from baseType to this type. parameter should be of base type, otherwise ill-formed
    };

    // Type = containing type
    // Data = pointer to member variable
    template<typename Type, auto Data>
    std::any getProperty(std::any value)
    {
        auto* v = std::any_cast<Type*>(value);
        return &(std::invoke(Data, v));
    }

    //-----------------------------------------------------
    // Register property
    //-----------------------------------------------------

    template<typename Type>
    size_t addNode(ClassInfo& info)
    {
        PropertyNode node{};
        if constexpr (IsVector<Type>::value)
        {
            node.type = PropertyNode::Type::Vector;
            node.list.valueNode = addNode<typename Type::value_type>(info);
            node.list.handle = AnyVectorHandler<Type>::handle;
        }
        else if constexpr (IsUnorderedMap<Type>::value)
        {
            node.type = PropertyNode::Type::UnorderedMap;
            node.dictionary.keyTypeId = TypeIndex<typename Type::key_type>::value();
            node.dictionary.valueNode = addNode<typename Type::mapped_type>(info);
            node.dictionary.handle = AnyUnorderedMapHandler<Type>::handle;
        }
        else if constexpr (IsUniquePointer<Type>::value)
        {
            node.type = PropertyNode::Type::UniquePtr;
            node.pointer.valueTypeId = TypeIndex<typename Type::element_type>::value();
        }
        else
        {
            node.type = PropertyNode::Type::Type;
            node.object.typeId = TypeIndex<Type>::value();
        }

        size_t index = info.nodes.size();
        info.nodes.emplace_back(node);
        return index;
    };

    //-----------------------------------------------------
    // Polymorphism
    //-----------------------------------------------------

    template<typename Type, typename BaseType>
    bool isType(std::any value)
    {
        auto** v = std::any_cast<BaseType*>(&value);
        assert(v);
        return dynamic_cast<Type*>(*v);
    }

    // should only be called if we know the downcast is possible
    template<typename Type, typename BaseType>
    std::any downcast(std::any value)
    {
        // don't need to downcast as it already is this type
        if (std::any_cast<Type*>(&value))
        {
            return value;
        }

        // otherwise, downcast
        auto** v = std::any_cast<BaseType*>(&value);
        assert(v);
        return dynamic_cast<Type*>(*v);
    }

    //-----------------------------------------------------
    // Builder
    //-----------------------------------------------------

    namespace register_
    {
        template<typename Type>
        class Class final
        {
        public:
            explicit Class(std::string name) : info(std::make_unique<ClassInfo>(std::move(name))) {}

            // Data is pointer to member variable
            template<auto Data>
            [[nodiscard]] Class& member(std::string const& name)
            {
                using PropertyType = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;
                size_t root = addNode<PropertyType>(*info);

                info->properties.emplace_back(PropertyInfo{
                    .name = name,
                    .node = root,
                    .get = getProperty<Type, Data>
                });

                return *this;
            }

            template<typename BaseType>
            [[nodiscard]] Class& base()
            {
                info->base = TypeIndex<BaseType>::value();
                info->isType = isType<Type, BaseType>;
                info->downcast = downcast<Type, BaseType>;
                return *this;
            }

            void emplace(TypeRegistry& types)
            {
                types.emplace<Type>(std::move(info));
            }

        private:
            std::unique_ptr<ClassInfo> info;
        };
    }
}

#endif //SHAPEREALITY_CLASS_H
