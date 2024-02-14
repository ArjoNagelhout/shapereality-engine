//
// Created by Arjo Nagelhout on 07/02/2024.
//

#include "type_info.h"

using namespace nlohmann;

namespace reflection
{
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
}