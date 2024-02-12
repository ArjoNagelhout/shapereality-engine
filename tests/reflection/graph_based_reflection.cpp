//
// Created by Arjo Nagelhout on 11/02/2024.
//

#include <gtest/gtest.h>

#include <reflection/type_id.h>

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
        std::vector<TypeNode> nodes; // container that owns all nodes that describe this type
        std::vector<Property> properties;
    };

    struct Property
    {
        std::string name; // name of property
        size_t node; // index to node of property
    };

    struct ObjectNode
    {
        type_id typeId; // type
    };

    struct ListNode
    {
        size_t valueNode; // index to TypeNode, Value of std::vector<Value>
    };

    struct DictionaryNode
    {
        type_id key; // Key of std::unordered_map<Key, Value>
        size_t valueNode; // index to TypeNode, Value of std::unordered_map
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

    using Registry = std::unordered_map<type_id, TypeInfo>;

    template<typename Type>
    void add(Registry& r, TypeInfo&& info)
    {
        r[TypeIndex<Type>::value()] = std::move(info);
    }

    template<typename Type>
    size_t addNode(TypeInfo& info)
    {
        TypeNode node{};
        if constexpr (is_list<Type>::value)
        {
            node.type = TypeNode::Type::List;
            node.list.valueNode = addNode<typename Type::value_type>(info);
        }
        else if constexpr (is_dictionary<Type>::value)
        {
            node.type = TypeNode::Type::Dictionary;
            node.dictionary.key = TypeIndex<typename Type::key_type>::value();
            node.dictionary.valueNode = addNode<typename Type::mapped_type>(info);
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

    // Data is pointer to member variable
    template<typename Type, auto Data>
    void property(TypeInfo& info, std::string name)
    {
        using property_type = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;
        size_t root = addNode<property_type>(info);

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

    struct StackFrame
    {

    };

    void object(Registry& r, type_id typeId, std::any const& value);

    void node(Registry& r, TypeInfo& info, size_t index, std::any const& value)
    {
        TypeNode& n = info.nodes[index];
        switch (n.type)
        {
            case TypeNode::Type::Object:
            {
                std::cout << "object" << std::endl;
                object(r, n.object.typeId, value);
                break;
            }
            case TypeNode::Type::List:
            {
                std::cout << "list" << std::endl;
                node(r, info, n.list.valueNode, value);
                break;
            }
            case TypeNode::Type::Dictionary:
            {
                std::cout << "dictionary" << std::endl;
                node(r, info, n.dictionary.valueNode, value);
                break;
            }
        }
    }

    // value = pointer to value
    void object(Registry& r, type_id typeId, std::any const& value)
    {
        TypeInfo& info = r[typeId];
        std::cout << info.name << std::endl;
        for (auto& property: info.properties)
        {
            node(r, info, property.node, value);
        }
    }

    TEST(Reflection, GraphBasedReflection)
    {
        Registry r;

        add<int>(r, {.name = "int"});
        add<float>(r, {.name = "float"});

        TypeInfo dataInfo{.name = "Data"};
        property<Data, &Data::data>(dataInfo, "data");
        property<Data, &Data::silly>(dataInfo, "silly");
        add<Data>(r, std::move(dataInfo));

        TypeInfo data2Info{.name = "Data2"};
        property<Data2, &Data2::myValues>(data2Info, "myValues");
        add<Data2>(r, std::move(data2Info));

        Data data;
        object(r, TypeIndex<Data>::value(), &data);
    }
}