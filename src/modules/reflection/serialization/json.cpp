//
// Created by Arjo Nagelhout on 14/02/2024.
//

#include "json.h"

#include <utility>

namespace reflection::json
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
    void emplaceBuiltIn(Converter& converter)
    {
        converter.emplace<Type>({.fromJson = builtInFromJson<Type>, .toJson = builtInToJson<Type>});
    }

    Converter::Converter()
    {
        // built-in types (automatically interpreted by the nlohmann::json library)
        emplaceBuiltIn<bool>(*this);
        emplaceBuiltIn<int>(*this);
        emplaceBuiltIn<float>(*this);
        emplaceBuiltIn<double>(*this);
        emplaceBuiltIn<std::string>(*this);
    }

    bool Converter::fromJson(type_id typeId, nlohmann::json const& in, std::any out)
    {
        if (!functions.contains(typeId))
        {
            return false;
        }
        functions[typeId].fromJson(in, std::move(out));
        return true;
    }

    bool Converter::toJson(type_id typeId, std::any const& in, nlohmann::json& out)
    {
        if (!functions.contains(typeId))
        {
            return false;
        }
        functions[typeId].toJson(in, out);
        return true;
    }

    //-----------------------------------------------------
    // From JSON
    //-----------------------------------------------------

    void objectFromJson(TypeInfoRegistry& r, Converter& converter,
                        nlohmann::json const& in, std::any out, type_id typeId)
    {
        TypeInfo* info = r.get(typeId);
        if (info == nullptr)
        {
            return;
        }

        bool converted = converter.fromJson(typeId, in, out);
        if (converted)
        {
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
            nodeFromJson(r, converter, propertyIn, propertyOut, *info, property.node);
        }
    }

    void nodeFromJson(TypeInfoRegistry& r, Converter& converter,
                      nlohmann::json const& in, std::any out, TypeInfo& info, size_t nodeIndex)
    {
        TypeNode& n = info.nodes[nodeIndex];
        switch (n.type)
        {
            case TypeNode::Type::Object:
            {
                objectFromJson(r, converter, in, out, n.object.typeId);
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
                    nodeFromJson(r, converter, listIn, listOut, info, n.list.valueNode);
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
                    nodeFromJson(r, converter, dictionaryIn, dictionaryOut, info, n.dictionary.valueNode);
                }
                break;
            }
        }
    }

    //-----------------------------------------------------
    // To JSON
    //-----------------------------------------------------

    void objectToJson(TypeInfoRegistry& r, Converter& converter,
                      std::any in, nlohmann::json& out, type_id typeId)
    {
        TypeInfo* info = r.get(typeId);
        if (info == nullptr)
        {
            return;
        }

        bool converted = converter.toJson(typeId, in, out);
        if (converted)
        {
            // we found a primitive type, we don't have to iterate over properties
            // as we assume these were not registered
            return;
        }

        for (auto& property: info->properties)
        {
            std::any propertyIn = property.get(in);
            out[property.name] = nlohmann::json::object();
            nlohmann::json& propertyOut = out[property.name];
            nodeToJson(r, converter, propertyIn, propertyOut, *info, property.node);
        }
    }

    void nodeToJson(TypeInfoRegistry& r, Converter& converter,
                    std::any in, nlohmann::json& out, TypeInfo& info, size_t nodeIndex)
    {
        TypeNode& n = info.nodes[nodeIndex];
        switch (n.type)
        {
            case TypeNode::Type::Object:
            {
                objectToJson(r, converter, in, out, n.object.typeId);
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
                    nodeToJson(r, converter, listIn, listOut, info, n.list.valueNode);
                }
                break;
            }
            case TypeNode::Type::Dictionary:
            {
                n.dictionary.iterate(in, [&](std::string const& key, std::any dictionaryIn) {
                    // for each dictionary entry, add a json object
                    out[key] = nlohmann::json::object();
                    nlohmann::json& dictionaryOut = out[key];
                    nodeToJson(r, converter, dictionaryIn, dictionaryOut, info, n.dictionary.valueNode);
                });
                break;
            }
        }
    }
}