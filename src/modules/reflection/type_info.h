//
// Created by Arjo Nagelhout on 04/02/2024.
//

#ifndef SHAPEREALITY_TYPE_INFO_H
#define SHAPEREALITY_TYPE_INFO_H

#include <reflection/type_id.h>

#include <utility>
#include <unordered_map>
#include <vector>
#include <any>
#include <string>
#include <functional>

/**
 * @namespace reflection
 * @brief simple reflection of plain-old datastructures (POD) or compositions of PODs
 */
namespace reflection
{
    // Reflection system for inspecting C++ POD structs on runtime,
    // and serializing and deserializing to and from json using nlohmann's json library
    //
    // Supported:
    // - nested std::vector<T> and std::unordered_map<Key, Value>
    // - nested POD structs
    //
    // Not supported and unplanned:
    // - inheritance
    // - function reflection
    //
    // Types and its properties can be registered using a TypeInfoBuilder or by manually
    // creating a TypeInfo, and then emplacing it in the TypeInfoRegistry.
    //
    // Internally, when registering a property, it uses recursive template instantiation
    // to create a hierarchy of TypeNodes:
    //
    // A std::vector<std::unordered_map<int, std::string>> property becomes
    // ListNode > DictionaryNode > ObjectNode (std::string).
    //
    // Each TypeInfo stores its own node hierarchy

    // helpers for detecting whether something is a std::vector or std::unordered_map
    template<typename>
    struct is_list : std::false_type
    {
    };

    template<typename Type>
    struct is_list<std::vector<Type>> : std::true_type
    {
    };

    template<typename>
    struct is_dictionary : std::false_type
    {
    };

    template<typename Key, typename Value>
    struct is_dictionary<std::unordered_map<Key, Value>> : std::true_type
    {
    };

    template<typename>
    struct is_pointer : std::false_type
    {
    };

    template<typename Type>
    struct is_pointer<std::unique_ptr<Type>> : std::false_type
    {
    };

    struct PropertyInfo final
    {
        std::string name; // name of property
        size_t node; // index to node of property (we don't use pointers so that TypeInfo is trivially copyable)

        std::any (* get)(std::any); // function pointer to get pointer to value of this property
    };

    struct ObjectNode final
    {
        TypeId typeId; // type
    };

    struct ListNode final
    {
        size_t valueNode; // index to TypeNode, Value of std::vector<Value>

        // first parameter (std::any) is the pointer to the std::vector we want to get the size of
        size_t (* size)(std::any); // pointer to function to get size of std::vector

        void (* resize)(std::any, size_t); // pointer to function to set size of std::vector

        std::any (* at)(std::any, size_t); // pointer to function to get value at index of std::vector
    };

    struct DictionaryNode final
    {
        TypeId keyTypeId; // Key of std::unordered_map<Key, Value>
        size_t valueNode; // index to TypeNode, Value of std::unordered_map

        using IterateCallback = std::function<void(std::string const&, std::any)>; // parameters: key, value

        void (* addKey)(std::any, std::string const&); // adds a key to the std::unordered_map

        void (* iterate)(std::any, IterateCallback const&);

        std::any (* at)(std::any, std::string const&); // get at key

        void (* clear)(std::any); // clear dictionary
    };

    struct PointerNode final
    {
        // we don't aim to support a pointer inside a pointer, or pointer to a dictionary as that would make no sense
        // therefore, we can directly store the type id
        TypeId valueTypeId;

        void (* make)(std::any);
    };

    struct PropertyNode final
    {
        enum class Type
        {
            Object, // a type
            List, // std::vector
            Dictionary, // std::unordered_map
            Pointer // std::unique_ptr
        };

        Type type;

        union
        {
            ObjectNode object;
            ListNode list;
            DictionaryNode dictionary;
            PointerNode pointer;
        };
    };

    struct TypeInfo final
    {
        // name of type
        std::string name;

        // polymorphism
        TypeId base;
        std::vector<TypeId> children;

        // properties
        std::vector<PropertyNode> nodes; // container that owns all nodes that describe this type
        std::vector<PropertyInfo> properties; // list of properties inside this type
    };

    struct EnumInfo final
    {
        std::string name;
    };

    struct PrimitiveInfo final
    {
        std::string name;
    };

    // Type = containing type
    // Data = pointer to member variable
    template<typename Type, auto Data>
    std::any get(std::any value)
    {
        auto* v = std::any_cast<Type*>(value);
        return &(std::invoke(Data, v));
    }

    template<typename Type>
    size_t listSize(std::any value)
    {
        auto* v = std::any_cast<Type*>(value);
        return v->size();
    }

    template<typename Type>
    void listResize(std::any value, size_t size)
    {
        auto* v = std::any_cast<Type*>(value);
        v->resize(size);
    }

    template<typename Type>
    std::any listAt(std::any value, size_t index)
    {
        auto* v = std::any_cast<Type*>(value);
        return &((*v)[index]);
    }

    // converting keys for std::unordered_map
    template<typename Type>
    constexpr Type primitiveFromString(std::string const& string)
    {
        //@formatter:off
        if constexpr (std::is_same_v<Type, std::string>) { return string; }
        else if constexpr (std::is_same_v<Type, int>) { return std::stoi(string); }
        else if constexpr (std::is_same_v<Type, long>) { return std::stol(string); }
        else if constexpr (std::is_same_v<Type, unsigned long>) { return std::stoul(string); }
        else if constexpr (std::is_same_v<Type, long long>) { return std::stoll(string); }
        else if constexpr (std::is_same_v<Type, unsigned long long>) { return std::stoull(string); }
        else if constexpr (std::is_same_v<Type, float>) { return std::stof(string); }
        else if constexpr (std::is_same_v<Type, double>) { return std::stod(string); }
        else if constexpr (std::is_same_v<Type, long double>) { return std::stold(string); }
        //@formatter:on
    }

    template<typename Type>
    constexpr std::string primitiveToString(Type value)
    {
        if constexpr (std::is_same_v<Type, std::string>)
        {
            return value;
        }
        else
        {
            return std::to_string(value);
        }
    }

    template<typename Type>
    void dictionaryAddKey(std::any value, std::string const& key)
    {
        using mapped_type = Type::mapped_type;
        using key_type = Type::key_type;

        auto* v = std::any_cast<Type*>(value);
        auto k = primitiveFromString<key_type>(key);
        (*v)[k] = mapped_type{}; // default initialize for key
    }

    template<typename Type>
    void dictionaryIterate(std::any value, DictionaryNode::IterateCallback const& callback)
    {
        auto& v = *std::any_cast<Type*>(value);
        for (auto [key, entryValue]: v)
        {
            // convert key to string
            std::string string = primitiveToString(key);
            callback(string, &entryValue);
        }
    }

    template<typename Type>
    std::any dictionaryAt(std::any value, std::string const& key)
    {
        using key_type = Type::key_type;

        auto* v = std::any_cast<Type*>(value);
        auto k = primitiveFromString<key_type>(key);

        return &((*v)[k]);
    }

    template<typename Type>
    void dictionaryClear(std::any value)
    {
        auto* v = std::any_cast<Type*>(value);
        v->clear();
    }

    //-----------------------------------------------------
    // Register property
    //-----------------------------------------------------

    template<typename Type>
    size_t addNode(TypeInfo& info)
    {
        PropertyNode node{};
        if constexpr (is_list < Type > ::value)
        {
            node.type = PropertyNode::Type::List;
            node.list.valueNode = addNode<typename Type::value_type>(info);
            node.list.size = listSize<Type>;
            node.list.resize = listResize<Type>;
            node.list.at = listAt<Type>;
        }
        else if constexpr (is_dictionary < Type > ::value)
        {
            node.type = PropertyNode::Type::Dictionary;
            node.dictionary.keyTypeId = TypeIndex<typename Type::key_type>::value();
            node.dictionary.valueNode = addNode<typename Type::mapped_type>(info);
            node.dictionary.addKey = dictionaryAddKey<Type>;
            node.dictionary.iterate = dictionaryIterate<Type>;
            node.dictionary.at = dictionaryAt<Type>;
            node.dictionary.clear = dictionaryClear<Type>;
        }
        else
        {
            node.type = PropertyNode::Type::Object;
            node.object.typeId = TypeIndex<Type>::value();
        }

        size_t index = info.nodes.size();
        info.nodes.emplace_back(node);
        return index;
    };

    //-----------------------------------------------------
    // Builder
    //-----------------------------------------------------

    class TypeInfoRegistry;

    template<typename Type>
    class TypeInfoBuilder final
    {
    public:
        explicit TypeInfoBuilder(std::string name) : info({.name = name})
        {
        }

        // Data is pointer to member variable
        template<auto Data>
        TypeInfoBuilder& property(std::string name)
        {
            using property_type = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;
            size_t root = addNode<property_type>(info);

            info.properties.emplace_back(PropertyInfo{
                .name = std::move(name),
                .node = root,
                .get = get<Type, Data>
            });

            return *this;
        }

        void emplace(TypeInfoRegistry& r);

        // emplaces the built type info into the shared instance
        void emplace();

    private:
        TypeInfo info;
    };

    //-----------------------------------------------------
    // Registry
    //-----------------------------------------------------

    // the registry contains the types that are registered
    // this avoids having a global unordered map somewhere
    class TypeInfoRegistry final
    {
    public:
        explicit TypeInfoRegistry();

        // shared instance
        [[nodiscard]] static TypeInfoRegistry& shared();

        template<typename Type>
        void emplace(TypeInfo&& info)
        {
            TypeId typeId = TypeIndex<Type>::value();
            types.emplace(typeId, std::move(info));
        }

        [[nodiscard]] bool contains(TypeId typeId) const;

        template<typename Type>
        [[nodiscard]] bool contains() const
        {
            TypeId typeId = TypeIndex<Type>::value();
            return contains(typeId);
        }

        [[nodiscard]] TypeInfo* get(TypeId typeId);

        template<typename Type>
        [[nodiscard]] TypeInfo* get()
        {
            TypeId typeId = TypeIndex<Type>::value();
            return get(typeId);
        }

    private:
        std::unordered_map<TypeId, TypeInfo> types;
    };

    //-----------------------------------------------------
    // Register type
    //-----------------------------------------------------

    template<typename Type>
    void TypeInfoBuilder<Type>::emplace(TypeInfoRegistry& r)
    {
        r.emplace<Type>(std::move(info));
    }

    /**
     * emplace in shared registry
     */
    template<typename Type>
    void TypeInfoBuilder<Type>::emplace()
    {
        TypeInfoRegistry::shared().emplace<Type>(std::move(info));
    }
}

#endif //SHAPEREALITY_TYPE_INFO_H
