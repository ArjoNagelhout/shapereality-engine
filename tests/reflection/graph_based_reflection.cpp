//
// Created by Arjo Nagelhout on 11/02/2024.
//

#include <gtest/gtest.h>

#include <reflection/type_id.h>

#include <forward_list>
#include <vector>
#include <unordered_map>

// implementation of graph based reflection
// to support nested lists and dictionaries

// simple polymorphism using union

using namespace reflection;

namespace graph_based_reflection
{
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

    struct TypeNode;
    struct Property;

    // we use a forward_list because pointers don't get invalidated when it needs to reallocate (e.g. when using emplace)
    struct TypeInfo
    {
        std::string name; // name of type
        std::forward_list<TypeNode> nodes; // container that owns all nodes that describe this type
        std::vector<Property> properties;
    };

    struct Property
    {
        std::string name; // name of property
        TypeNode* node; // unowned pointer to node of property
    };

    struct ObjectNode
    {
        type_id typeId; // type
    };

    struct ListNode
    {
        TypeNode* value; // Value of std::vector<Value>
    };

    struct DictionaryNode
    {
        type_id key; // Key of std::unordered_map<Key, Value>
        TypeNode* value; // Value of std::unordered_map
    };

    struct TypeNode
    {
        enum class Type
        {
            Object,
            List,
            Dictionary
        };

//        std::any (* getter)(std::any); // function pointer to get pointer to reflected type of this node

        Type type;

        union
        {
            ObjectNode object;
            ListNode list;
            DictionaryNode dictionary;
        };
    };

    template<typename Type>
    TypeInfo createTypeInfo()
    {

    }

    using Registry = std::unordered_map<type_id, TypeInfo>;

    template<typename Type>
    void add(Registry& r, TypeInfo&& info)
    {
        r[TypeIndex<Type>::value()] = info;
    }

    template<typename Type>
    TypeNode* addNode(TypeInfo& info)
    {
        TypeNode node{};
        if constexpr (is_list<Type>::value)
        {
            node.type = TypeNode::Type::List;
            node.list.value = addNode<typename Type::value_type>(info);
        }
        else if constexpr (is_dictionary<Type>::value)
        {
            node.type = TypeNode::Type::Dictionary;
            node.dictionary.key = TypeIndex<typename Type::key_type>::value();
            node.dictionary.value = addNode<typename Type::mapped_type>(info);
        }
        else
        {
            node.type = TypeNode::Type::Object;
            node.object.typeId = TypeIndex<Type>::value();
        }

        return &(info.nodes.emplace_front(node));
    };

    // Data is pointer to member variable
    template<typename Type, auto Data>
    void property(TypeInfo& info, std::string name)
    {
        using property_type = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;
        TypeNode* root = addNode<property_type>(info);

        info.properties.emplace_back(Property{
            .name = std::move(name),
            .node = root
        });
    }

    struct Data2
    {
        std::unordered_map<std::string, std::vector<float>> myValues;
    };

    struct Data
    {
        std::vector<Data2> data;
        std::vector<std::vector<std::vector<std::unordered_map<int, std::vector<std::vector<float>>>>>> silly;
    };

    TEST(Reflection, GraphBasedReflection)
    {
        Registry r;

        TypeInfo info{
            .name = "Data"
        };
        std::cout << "data:" << std::endl;
        property<Data, &Data::data>(info, "data");
        std::cout << "silly:" << std::endl;
        property<Data, &Data::silly>(info, "silly");
        add<Data>(r, std::move(info));

        std::cout << "test" << std::endl;

        // create a
    }
}