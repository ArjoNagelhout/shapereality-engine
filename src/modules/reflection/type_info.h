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

    template<typename T>
    struct is_list<std::vector<T>> : std::true_type
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

    struct Property final
    {
        std::string name; // name of property
        size_t node; // index to node of property (we don't use pointers so that TypeInfo is trivially copyable)

        std::any (* get)(std::any); // function pointer to get pointer to value of this property
    };

    struct ObjectNode final
    {
        type_id typeId; // type
    };

    struct ListNode final
    {
        size_t valueNode; // index to TypeNode, Value of std::vector<Value>

        size_t (* size)(std::any); // pointer to function to get size of std::vector

        void (* resize)(std::any, size_t); // pointer to function to set size of std::vector

        std::any (* at)(std::any, size_t); // pointer to function to get value at index of std::vector
    };

    struct DictionaryNode final
    {
        type_id keyTypeId; // Key of std::unordered_map<Key, Value>
        size_t valueNode; // index to TypeNode, Value of std::unordered_map

        using iterate_callback = std::function<void(std::string const&, std::any)>; // parameters: key, value

        void (* addKey)(std::any, std::string const&); // adds a key to the std::unordered_map

        void (* iterate)(std::any, iterate_callback const&);

        std::any (* at)(std::any, std::string const&); // get at key

        void (* clear)(std::any); // clear dictionary
    };

    struct TypeNode final
    {
        enum class Type
        {
            Object,
            List,
            Dictionary
        };

        Type type;

        union
        {
            ObjectNode object;
            ListNode list;
            DictionaryNode dictionary;
        };
    };

    struct TypeInfo final
    {
        std::string name; // name of type
        std::vector<TypeNode> nodes; // container that owns all nodes that describe this type
        std::vector<Property> properties;
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
    size_t size(std::any value)
    {
        auto* v = std::any_cast<Type*>(value);
        return v->size();
    }

    template<typename Type>
    void resize(std::any value, size_t size)
    {
        auto* v = std::any_cast<Type*>(value);
        v->resize(size);
    }

    template<typename Type>
    std::any at(std::any value, size_t index)
    {
        auto* v = std::any_cast<Type*>(value);
        return &((*v)[index]);
    }

    // converting keys for std::unordered_map
    template<typename Type>
    constexpr Type fromString(std::string const& string)
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
    constexpr std::string toString(Type value)
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
    void addKey(std::any value, std::string const& key)
    {
        using mapped_type = Type::mapped_type;
        using key_type = Type::key_type;

        auto* v = std::any_cast<Type*>(value);
        auto k = fromString<key_type>(key);
        (*v)[k] = mapped_type{}; // default initialize for key
    }

    template<typename Type>
    void iterate(std::any value, DictionaryNode::iterate_callback const& callback)
    {
        auto& v = *std::any_cast<Type*>(value);
        for (auto [key, entryValue]: v)
        {
            // convert key to string
            std::string string = toString(key);
            callback(string, &entryValue);
        }
    }

    template<typename Type>
    std::any dictionaryAt(std::any value, std::string const& key)
    {
        using key_type = Type::key_type;

        auto* v = std::any_cast<Type*>(value);
        auto k = fromString<key_type>(key);

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
        TypeNode node{};
        if constexpr (is_list<Type>::value)
        {
            node.type = TypeNode::Type::List;
            node.list.valueNode = addNode<typename Type::value_type>(info);
            node.list.size = size<Type>;
            node.list.resize = resize<Type>;
            node.list.at = at<Type>;
        }
        else if constexpr (is_dictionary<Type>::value)
        {
            node.type = TypeNode::Type::Dictionary;
            node.dictionary.keyTypeId = TypeIndex<typename Type::key_type>::value();
            node.dictionary.valueNode = addNode<typename Type::mapped_type>(info);
            node.dictionary.addKey = addKey<Type>;
            node.dictionary.iterate = iterate<Type>;
            node.dictionary.at = dictionaryAt<Type>;
            node.dictionary.clear = dictionaryClear<Type>;
        }
        else
        {
            node.type = TypeNode::Type::Object;
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

            info.properties.emplace_back(Property{
                .name = std::move(name),
                .node = root,
                .get = get<Type, Data>
            });

            return *this;
        }

        void emplace(TypeInfoRegistry& r);

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
        template<typename Type>
        void emplace(TypeInfo&& info)
        {
            type_id typeId = TypeIndex<Type>::value();
            types.emplace(typeId, std::move(info));
        }

        [[nodiscard]] bool contains(type_id typeId) const;

        template<typename Type>
        [[nodiscard]] bool contains() const
        {
            type_id typeId = TypeIndex<Type>::value();
            return contains(typeId);
        }

        [[nodiscard]] TypeInfo* get(type_id typeId);

        template<typename Type>
        [[nodiscard]] TypeInfo* get()
        {
            type_id typeId = TypeIndex<Type>::value();
            return get(typeId);
        }

    private:
        std::unordered_map<type_id, TypeInfo> types;
    };

    //-----------------------------------------------------
    // Register type
    //-----------------------------------------------------

    template<typename Type>
    void TypeInfoBuilder<Type>::emplace(TypeInfoRegistry& r)
    {
        r.emplace<Type>(std::move(info));
    }
}

#endif //SHAPEREALITY_TYPE_INFO_H
