//
// Created by Arjo Nagelhout on 14/02/2024.
//

#include "json.h"

#include <utility>

namespace reflection
{
    //-----------------------------------------------------
    // Converter
    //-----------------------------------------------------

    template<typename Type>
    void builtInFromJson(nlohmann::json const& in, std::any out)
    {
        *std::any_cast<Type*>(out) = in.get<Type>();
    }

    template<typename Type>
    void builtInToJson(std::any in, nlohmann::json& out)
    {
        out = *std::any_cast<Type*>(in);
    }

    template<typename Type>
    void emplaceBuiltIn(JsonSerializer& serializer)
    {
        serializer.emplace<Type>({.from = builtInFromJson<Type>, .to = builtInToJson<Type>});
    }

    JsonSerializer::JsonSerializer(TypeInfoRegistry& _r) : r(_r)
    {
        // built-in types (automatically interpreted by the nlohmann::json library)
        emplaceBuiltIn<bool>(*this);
        emplaceBuiltIn<int>(*this);
        emplaceBuiltIn<float>(*this);
        emplaceBuiltIn<double>(*this);
        emplaceBuiltIn<std::string>(*this);
    }

    //-----------------------------------------------------
    // From JSON
    //-----------------------------------------------------

    void JsonSerializer::objectFromJson(nlohmann::json const& in, std::any out, type_id typeId)
    {
        TypeInfo* info = r.get(typeId);
        if (info == nullptr)
        {
            return;
        }

        if (functions.contains(typeId))
        {
            // we found a primitive type, we don't have to iterate over properties
            // as we assume these were not registered
            functions[typeId].from(in, out);
            return;
        }

        for (auto& property: info->properties)
        {
            if (!in.contains(property.name))
            {
                continue;
            }

            nlohmann::json const& propertyIn = in[property.name];
            std::any propertyOut = property.get(out);
            nodeFromJson(propertyIn, propertyOut, *info, property.node);
        }
    }

    void JsonSerializer::nodeFromJson(nlohmann::json const& in, std::any out, TypeInfo& info, size_t nodeIndex)
    {
        TypeNode& n = info.nodes[nodeIndex];
        switch (n.type)
        {
            case TypeNode::Type::Object:
            {
                objectFromJson(in, out, n.object.typeId);
                break;
            }
            case TypeNode::Type::List:
            {
                size_t size = in.size();
                n.list.resize(out, size);
                for (size_t i = 0; i < size; i++)
                {
                    nlohmann::json const& listIn = in[i];
                    std::any listOut = n.list.at(out, i);
                    nodeFromJson(listIn, listOut, info, n.list.valueNode);
                }
                break;
            }
            case TypeNode::Type::Dictionary:
            {
                n.dictionary.clear(out); // make sure no extraneous elements exist
                for (auto [key, value]: in.items())
                {
                    n.dictionary.addKey(out, key);
                    nlohmann::json const& dictionaryIn = value;
                    std::any dictionaryOut = n.dictionary.at(out, key);
                    nodeFromJson(dictionaryIn, dictionaryOut, info, n.dictionary.valueNode);
                }
                break;
            }
        }
    }

    //-----------------------------------------------------
    // To JSON
    //-----------------------------------------------------

    void JsonSerializer::objectToJson(std::any in, nlohmann::json& out, type_id typeId)
    {
        TypeInfo* info = r.get(typeId);
        if (info == nullptr)
        {
            return;
        }

        if (functions.contains(typeId))
        {
            // we found a primitive type, we don't have to iterate over properties
            // as we assume these were not registered
            functions[typeId].to(in, out);
            return;
        }

        for (auto& property: info->properties)
        {
            std::any propertyIn = property.get(in);
            out[property.name] = nlohmann::json::object();
            nlohmann::json& propertyOut = out[property.name];
            nodeToJson(propertyIn, propertyOut, *info, property.node);
        }
    }

    void JsonSerializer::nodeToJson(std::any in, nlohmann::json& out, TypeInfo& info, size_t nodeIndex)
    {
        TypeNode& n = info.nodes[nodeIndex];
        switch (n.type)
        {
            case TypeNode::Type::Object:
            {
                objectToJson(in, out, n.object.typeId);
                break;
            }
            case TypeNode::Type::List:
            {
                out = nlohmann::json::array(); // convert to array
                size_t size = n.list.size(in);
                for (size_t i = 0; i < size; i++)
                {
                    std::any listIn = n.list.at(in, i);
                    nlohmann::json& listOut = out.emplace_back(nlohmann::json::object());
                    nodeToJson(listIn, listOut, info, n.list.valueNode);
                }
                break;
            }
            case TypeNode::Type::Dictionary:
            {
                n.dictionary.iterate(in, [&](std::string const& key, std::any dictionaryIn) {
                    // for each dictionary entry, add a json object
                    out[key] = nlohmann::json::object();
                    nlohmann::json& dictionaryOut = out[key];
                    nodeToJson(dictionaryIn, dictionaryOut, info, n.dictionary.valueNode);
                });
                break;
            }
        }
    }
}