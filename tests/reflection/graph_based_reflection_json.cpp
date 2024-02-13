//
// Created by Arjo Nagelhout on 11/02/2024.
//

#include <gtest/gtest.h>

#include <reflection/type_id.h>

#include <stack>
#include <utility>
#include <vector>
#include <unordered_map>

#include <nlohmann/json.hpp>

// implementation of graph based reflection
// to support nested lists and dictionaries

// simple polymorphism using union

using namespace nlohmann;
using namespace reflection;

namespace graph_based_reflection_json
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

        std::any (* get)(std::any); // function pointer to get pointer to value of this property
    };

    struct ObjectNode
    {
        type_id typeId; // type
    };

    struct ListNode
    {
        size_t valueNode; // index to TypeNode, Value of std::vector<Value>

        size_t (* size)(std::any); // pointer to function to get size of std::vector

        std::any (* at)(std::any, size_t); // pointer to function to get value at index of std::vector
    };

    struct DictionaryNode
    {
        type_id key; // Key of std::unordered_map<Key, Value>
        size_t valueNode; // index to TypeNode, Value of std::unordered_map

        using iterate_callback = std::function<void(std::string const&, std::any)>; // parameters: key, value

        void (* iterate)(std::any, iterate_callback const&);
    };

    struct TypeNode
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

    using Registry = std::unordered_map<type_id, TypeInfo>;

    template<typename Type>
    void add(Registry& r, TypeInfo&& info)
    {
        r[TypeIndex<Type>::value()] = std::move(info);
    }

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
    std::any at(std::any value, size_t index)
    {
        auto* v = std::any_cast<Type*>(value);
        return &((*v)[index]);
    }

    template<typename Type>
    void iterate(std::any value, DictionaryNode::iterate_callback const& callback)
    {
        auto& v = *std::any_cast<Type*>(value);
        for (auto [key, entryValue]: v)
        {
            // convert key to string
            std::string string;
            if constexpr (std::is_same_v<typename Type::key_type, std::string>)
            {
                string = key;
            }
            else
            {
                string = std::to_string(key);
            }
            callback(string, &entryValue);
        }
    }

    template<typename Type>
    size_t addNode(TypeInfo& info)
    {
        TypeNode node{};
        if constexpr (is_list<Type>::value)
        {
            node.type = TypeNode::Type::List;
            node.list.valueNode = addNode<typename Type::value_type>(info);
            node.list.size = size<Type>;
            node.list.at = at<Type>;
        }
        else if constexpr (is_dictionary<Type>::value)
        {
            node.type = TypeNode::Type::Dictionary;
            node.dictionary.key = TypeIndex<typename Type::key_type>::value();
            node.dictionary.valueNode = addNode<typename Type::mapped_type>(info);
            node.dictionary.iterate = iterate<Type>;
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
            .node = root,
            .get = get<Type, Data>
        });
    }

    enum class ReflectCallbackType
    {
        Property,
        List,
        Value,
        Pop
    };

    struct ReflectCallbackData
    {
        ReflectCallbackType type;
        std::string name;
        type_id typeId;
        std::any value;
    };

    using reflect_callback = std::function<void(ReflectCallbackData const&)>;

    void reflectObject(Registry&, type_id, std::any, reflect_callback const&);

    void reflectNode(Registry& r, TypeInfo& info, size_t index, std::any value, reflect_callback const& callback)
    {
        TypeNode& n = info.nodes[index];

        switch (n.type)
        {
            case TypeNode::Type::Object:
            {
                reflectObject(r, n.object.typeId, std::move(value), callback); // we can simply pass the value
                break;
            }
            case TypeNode::Type::List:
            {
                callback({ReflectCallbackType::List}); // begin list

                size_t size = n.list.size(value);
                for (size_t i = 0; i < size; i++)
                {
                    std::any v = n.list.at(value, i);
                    reflectNode(r, info, n.list.valueNode, v, callback);
                }

                callback({ReflectCallbackType::Pop}); // end list
                break;
            }
            case TypeNode::Type::Dictionary:
            {
                callback({ReflectCallbackType::Value});
                n.dictionary.iterate(value, [&](std::string const& key, std::any value) {
                    callback({.type = ReflectCallbackType::Property, .name = key}); // "property_name": {
                    reflectNode(r, info, n.dictionary.valueNode, std::move(value), callback);
                    callback({ReflectCallbackType::Pop}); // }
                });
                callback({ReflectCallbackType::Pop});
                break;
            }
        }
    }

    // value = pointer to value
    void reflectObject(Registry& r, type_id typeId, std::any value, reflect_callback const& callback)
    {
        TypeInfo& info = r[typeId];

        callback({.type = ReflectCallbackType::Value, .typeId = typeId, .value = value});
        for (auto& property: info.properties)
        {
            callback({.type = ReflectCallbackType::Property, .name = property.name});
            reflectNode(r, info, property.node, property.get(value), callback);
            callback({ReflectCallbackType::Pop});
        }
        callback({ReflectCallbackType::Pop});
    }

    // convert std::any value to json
    json toJson(std::any value, type_id id)
    {
        if (!value.has_value())
        {
            return json::object();
        }

        if (isType<float>(id))
        {
            return *std::any_cast<float*>(value);
        }
        else if (isType<int>(id))
        {
            return *std::any_cast<int*>(value);
        }
        else if (isType<std::string>(id))
        {
            return *std::any_cast<std::string*>(value);
        }
        else if (isType<bool>(id))
        {
            return *std::any_cast<bool*>(value);
        }
        else if (isType<double>(id))
        {
            return *std::any_cast<double*>(value);
        }
        else
        {
            return json::object();
        }
    }

    template<typename Type>
    std::string toJson(Registry& r, Type& value)
    {
        json out;
        std::stack<json*> stack;
        stack.emplace(&out);

        type_id typeId = TypeIndex<Type>::value();
        reflectObject(r, typeId, &value, [&](ReflectCallbackData const& d) {
            json& top = *stack.top();
            switch (d.type)
            {
                case ReflectCallbackType::Property:
                {
                    auto [it, _] = top.emplace(d.name, json::object());
                    stack.push(&*it);
                    break;
                }
                case ReflectCallbackType::List:
                {
                    top = json::array(); // convert the top to an array
                    stack.push(&top);
                    break;
                }
                case ReflectCallbackType::Value:
                {
                    json value = toJson(d.value, d.typeId);
                    if (top.is_array())
                    {
                        stack.push(&top.emplace_back(value));
                    }
                    else
                    {
                        top = value;
                        stack.push(&top);
                    }
                    break;
                }
                case ReflectCallbackType::Pop:
                {
                    stack.pop();
                    break;
                }
            }
        });

        return out.dump(4);
    }

    struct Data2;

    struct Data
    {
        std::vector<Data2> data;
        std::vector<std::vector<std::vector<std::unordered_map<int, std::vector<std::vector<float>>>>>> silly;
    };

    struct Data3
    {
        float a = 1.3f;
        bool b = false;
        int c = 1346;
        double d = 1.6;
        std::string e = "yes yes";
    };

    struct Data2
    {
        std::unordered_map<std::string, std::vector<float>> myValues;

        std::unordered_map<std::string, Data3> data3s;
    };

    TEST(Reflection, GraphBasedReflectionJson)
    {
        Registry r;

        add<int>(r, {.name = "int"});
        add<float>(r, {.name = "float"});
        add<bool>(r, {.name = "bool"});
        add<double>(r, {.name = "double"});
        add<std::string>(r, {.name = "string"});

        TypeInfo data3Info{.name = "Data3"};
        property<Data3, &Data3::a>(data3Info, "a");
        property<Data3, &Data3::b>(data3Info, "b");
        property<Data3, &Data3::c>(data3Info, "c");
        property<Data3, &Data3::d>(data3Info, "d");
        property<Data3, &Data3::e>(data3Info, "e");
        add<Data3>(r, std::move(data3Info));

        TypeInfo dataInfo{.name = "Data"};
        property<Data, &Data::data>(dataInfo, "data");
        property<Data, &Data::silly>(dataInfo, "silly");
        add<Data>(r, std::move(dataInfo));

        TypeInfo data2Info{.name = "Data2"};
        property<Data2, &Data2::myValues>(data2Info, "myValues");
        property<Data2, &Data2::data3s>(data2Info, "data3s");
        add<Data2>(r, std::move(data2Info));

        Data data{
            .data = {
                Data2{
                    .myValues{
                        {"something", {0.1f, 0.2f, 0.3f}},
                        {"second", {1.2f, 1.3f}},
                        {"third", {1.0f}}
                    }
                },
                Data2{
                    .myValues{
                        {"wow", {1.0f}}
                    },
                    .data3s{
                        {
                            "asdflkajsdf", Data3{
                            .a = 1.f
                        }},
                        {
                            "oezoe", Data3{
                            .b = true
                        }},
                        {
                            "owoe", Data3{
                            .c = 12342384
                        }},
                        {
                            "beng", Data3{
                            .e = "no no no"
                        }}
                    }
                }
            }
        };

        std::cout << toJson<Data>(r, data) << std::endl;
    }
}