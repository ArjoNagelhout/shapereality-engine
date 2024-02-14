//
// Created by Arjo Nagelhout on 11/02/2024.
//

#include <gtest/gtest.h>

#include <reflection/type_id.h>
#include <nlohmann/json.hpp>

#include <utility>
#include <unordered_map>
#include <vector>
#include <any>
#include <string>
#include <iostream>

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

    struct Property
    {
        std::string name; // name of property
        size_t node; // index to node of property (we don't use pointers so that TypeInfo is trivially copyable)

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

        void (* resize)(std::any, size_t); // pointer to function to set size of std::vector

        std::any (* at)(std::any, size_t); // pointer to function to get value at index of std::vector
    };

    struct DictionaryNode
    {
        type_id keyTypeId; // Key of std::unordered_map<Key, Value>
        size_t valueNode; // index to TypeNode, Value of std::unordered_map

        using iterate_callback = std::function<void(std::string const&, std::any)>; // parameters: key, value

        void (* addKey)(std::any, std::string const&); // adds a key to the std::unordered_map

        void (* iterate)(std::any, iterate_callback const&);

        std::any (* at)(std::any, std::string const&); // get at key

        void (* clear)(std::any); // clear dictionary
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

    struct TypeInfo
    {
        std::string name; // name of type
        std::vector<TypeNode> nodes; // container that owns all nodes that describe this type
        std::vector<Property> properties;
    };

    using Registry = std::unordered_map<type_id, TypeInfo>;

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

    template<typename Type>
    void add(Registry& r, TypeInfo&& info)
    {
        type_id typeId = TypeIndex<Type>::value();
        std::cout << "typeId: " << typeId << std::endl;
        r[typeId] = std::move(info);
    }

    // Data is pointer to member variable
    template<typename Type, auto Data>
    void property(TypeInfo& info, std::string name)
    {
        using property_type = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;
        size_t root = addNode<property_type>(info);

        std::cout << "add property" << name << std::endl;
        info.properties.emplace_back(Property{
            .name = name,
            .node = root,
            .get = get<Type, Data>
        });
    }

    // serialization from and to json using type info

    //-----------------------------------------------------
    // To JSON
    //-----------------------------------------------------

    void objectToJson(Registry& r, std::any in, nlohmann::json& out, type_id typeId);

    void nodeToJson(Registry& r, std::any in, nlohmann::json& out, TypeInfo& info, size_t nodeIndex);

    template<typename Type>
    nlohmann::json toJson(Registry& r, Type& in)
    {
        nlohmann::json out = nlohmann::json::object();
        objectToJson(r, &in, out, TypeIndex<Type>::value());
        return out;
    }

    //-----------------------------------------------------
    // From JSON
    //-----------------------------------------------------

    void objectFromJson(Registry& r, nlohmann::json const& in, std::any out, type_id typeId);

    void nodeFromJson(Registry& r, nlohmann::json const& in, std::any out, TypeInfo& info, size_t nodeIndex);

    template<typename Type>
    void fromJson(Registry& r, nlohmann::json const& in, Type& out)
    {
        objectFromJson(r, in, &out, TypeIndex<Type>::value());
    }

    template<typename Type>
    Type fromJson(Registry& r, std::string const& in)
    {
        nlohmann::json parsed = nlohmann::json::parse(in);
        Type out;
        fromJson(r, parsed, out);
        return out;
    }

    //-----------------------------------------------------
    // From JSON
    //-----------------------------------------------------

    template<typename Type>
    void valueFromJson(nlohmann::json const& in, std::any out)
    {
        *std::any_cast<Type*>(out) = in.get<Type>();
    }

    // set the value from json
    void valueFromJson(nlohmann::json const& in, std::any out, type_id id)
    {
        //@formatter:off
        if (isType<int>(id)) { valueFromJson<int>(in, out); }
        else if (isType<float>(id)) { valueFromJson<float>(in, out); }
        else if (isType<bool>(id)) { valueFromJson<bool>(in, out); }
        else if (isType<std::string>(id)) { valueFromJson<std::string>(in, out); }
        else if (isType<double>(id)) { valueFromJson<double>(in, out); }
        //@formatter:on
    }

    void objectFromJson(Registry& r, json const& in, std::any out, type_id typeId)
    {
        TypeInfo& info = r[typeId];

        valueFromJson(in, out, typeId);

        for (auto& property: info.properties)
        {
            if (!in.contains(property.name))
            {
                continue;
            }

            json const& propertyIn = in[property.name];
            std::any propertyOut = property.get(out);
            nodeFromJson(r, propertyIn, propertyOut, info, property.node);
        }
    }

    void nodeFromJson(Registry& r, json const& in, std::any out, TypeInfo& info, size_t nodeIndex)
    {
        TypeNode& n = info.nodes[nodeIndex];
        switch (n.type)
        {
            case TypeNode::Type::Object:
            {
                objectFromJson(r, in, out, n.object.typeId);
                break;
            }
            case TypeNode::Type::List:
            {
                size_t size = in.size();
                n.list.resize(out, size);
                for (size_t i = 0; i < size; i++)
                {
                    json const& listIn = in[i];
                    std::any listOut = n.list.at(out, i);
                    nodeFromJson(r, listIn, listOut, info, n.list.valueNode);
                }
                break;
            }
            case TypeNode::Type::Dictionary:
            {
                n.dictionary.clear(out); // make sure no extraneous elements exist
                for (auto [key, value]: in.items())
                {
                    n.dictionary.addKey(out, key);
                    json const& dictionaryIn = value;
                    std::any dictionaryOut = n.dictionary.at(out, key);
                    nodeFromJson(r, dictionaryIn, dictionaryOut, info, n.dictionary.valueNode);
                }
                break;
            }
        }
    }

    //-----------------------------------------------------
    // To JSON
    //-----------------------------------------------------

    template<typename Type>
    void valueToJson(std::any in, nlohmann::json& out)
    {
        out = *std::any_cast<Type*>(in);
    }

    // set the value from json
    void valueToJson(std::any in, nlohmann::json& out, type_id id)
    {
        //@formatter:off
        if (isType<int>(id)) { valueToJson<int>(in, out); }
        else if (isType<float>(id)) { valueToJson<float>(in, out); }
        else if (isType<bool>(id)) { valueToJson<bool>(in, out); }
        else if (isType<std::string>(id)) { valueToJson<std::string>(in, out); }
        else if (isType<double>(id)) { valueToJson<double>(in, out); }
        //@formatter:on
    }

    void objectToJson(Registry& r, std::any in, json& out, type_id typeId)
    {
        TypeInfo& info = r[typeId];

        valueToJson(in, out, typeId);

        for (auto& property: info.properties)
        {
            std::cout << "what tueasdf cucucuc" << std::endl;
            std::cout << "property name: " << property.name << std::endl;
            std::any propertyIn = property.get(in);
            out[property.name] = json::object();
            json& propertyOut = out[property.name];
            nodeToJson(r, propertyIn, propertyOut, info, property.node);
        }
    }

    void nodeToJson(Registry& r, std::any in, json& out, TypeInfo& info, size_t nodeIndex)
    {
        TypeNode& n = info.nodes[nodeIndex];
        switch (n.type)
        {
            case TypeNode::Type::Object:
            {
                objectToJson(r, in, out, n.object.typeId);
                break;
            }
            case TypeNode::Type::List:
            {
                out = json::array(); // convert to array
                size_t size = n.list.size(in);
                for (size_t i = 0; i < size; i++)
                {
                    std::any listIn = n.list.at(in, i);
                    json& listOut = out.emplace_back(json::object());
                    nodeToJson(r, listIn, listOut, info, n.list.valueNode);
                }
                break;
            }
            case TypeNode::Type::Dictionary:
            {
                n.dictionary.iterate(in, [&](std::string const& key, std::any dictionaryIn) {
                    // for each dictionary entry, add a json object
                    out[key] = json::object();
                    json& dictionaryOut = out[key];
                    nodeToJson(r, dictionaryIn, dictionaryOut, info, n.dictionary.valueNode);
                });
                break;
            }
        }
    }

    //-----------------------------------------------------
    // Test case
    //-----------------------------------------------------

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

    struct Data
    {
        std::vector<Data2> data;
        std::vector<std::vector<std::vector<std::unordered_map<int, std::vector<std::vector<float>>>>>> silly;
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

        json out = toJson<Data>(r, data);
        std::cout << out.dump(2) << std::endl;

        std::string in = R"({"data":[{"data3s":{},"myValues":{"second":[1.2000000476837158,1.2999999523162842],"something":[0.10000000149011612,0.20000000298023224,0.30000001192092896],"third":[1.0]}},{"data3s":{"asdflkajsdf":{"a":1.0,"b":false,"c":1346,"d":1.6,"e":"yes yes"},"beng":{"a":1.2999999523162842,"b":false,"c":1346,"d":1.6,"e":"no no no"},"oezoe":{"a":1.2999999523162842,"b":true,"c":1346,"d":1.6,"e":"yes yes"},"owoe":{"a":1.2999999523162842,"b":false,"c":12342384,"d":1.6,"e":"yes yes"}},"myValues":{"wow":[1.0]}}],"silly":[]})";
        Data result = fromJson<Data>(r, in);
    }
}