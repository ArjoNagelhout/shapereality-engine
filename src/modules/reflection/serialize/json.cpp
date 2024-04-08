//
// Created by Arjo Nagelhout on 14/02/2024.
//

#include "json.h"

#include <reflection/class.h>

namespace reflection
{
    //-----------------------------------------------------
    // Serializer
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

    JsonSerializer::JsonSerializer(TypeInfoRegistry& r_, EnumSerializer& enums_) : r(r_), enums(enums_)
    {
        // built-in types (automatically interpreted by the nlohmann::json library)
        emplaceBuiltIn<bool>(*this);
        emplaceBuiltIn<int>(*this);
        emplaceBuiltIn<float>(*this);
        emplaceBuiltIn<double>(*this);
        emplaceBuiltIn<std::string>(*this);
    }

    JsonSerializer::~JsonSerializer() = default;

    JsonSerializer& JsonSerializer::shared()
    {
        static JsonSerializer instance_(TypeInfoRegistry::shared(), EnumSerializer::shared());
        return instance_;
    }

    //-----------------------------------------------------
    // From JSON
    //-----------------------------------------------------

    void JsonSerializer::typeFromJson(nlohmann::json const& in, std::any out, TypeId typeId)
    {
        TypeInfo* info = r.get(typeId);
        if (info == nullptr)
        {
            return;
        }

        if (enums.contains(typeId))
        {
            enums.anyFromString(in.get<std::string>(), out, typeId);
            return;
        }

        if (functions.contains(typeId))
        {
            // we found a primitive type, we don't have to iterate over properties
            // as we assume these were not registered
            functions[typeId].from(in, out);
            return;
        }

        if (info->type() == TypeInfo::Type::Class)
        {
            for (auto& property: info->class_().properties)
            {
                if (!in.contains(property.name))
                {
                    continue;
                }

                nlohmann::json const& propertyIn = in[property.name];
                std::any propertyOut = property.get(out);
                propertyNodeFromJson(propertyIn, propertyOut, info->class_(), property.node);
            }
        }
    }

    void JsonSerializer::propertyNodeFromJson(nlohmann::json const& in, std::any out, ClassInfo& info, size_t nodeIndex)
    {
        PropertyNode& n = info.nodes[nodeIndex];
        switch (n.type)
        {
            case PropertyNode::Type::Type:
            {
                typeFromJson(in, out, n.object.typeId);
                break;
            }
            case PropertyNode::Type::List:
            {
                size_t size = in.size();
                n.list.resize(out, size);
                for (size_t i = 0; i < size; i++)
                {
                    nlohmann::json const& listIn = in[i];
                    std::any listOut = n.list.at(out, i);
                    propertyNodeFromJson(listIn, listOut, info, n.list.valueNode);
                }
                break;
            }
            case PropertyNode::Type::Dictionary:
            {
                n.dictionary.clear(out); // make sure no extraneous elements exist
                for (auto [key, value]: in.items())
                {
                    n.dictionary.addKey(out, key);
                    nlohmann::json const& dictionaryIn = value;
                    std::any dictionaryOut = n.dictionary.at(out, key);
                    propertyNodeFromJson(dictionaryIn, dictionaryOut, info, n.dictionary.valueNode);
                }
                break;
            }
            case PropertyNode::Type::Pointer:
            {
                break;
            }
        }
    }

    //-----------------------------------------------------
    // To JSON
    //-----------------------------------------------------

    void JsonSerializer::typeToJson(std::any in, nlohmann::json& out, TypeId typeId)
    {
        TypeInfo* info = r.get(typeId);
        if (info == nullptr)
        {
            return;
        }

        if (enums.contains(typeId))
        {
            out = enums.anyToString(in, typeId);
            return;
        }

        if (functions.contains(typeId))
        {
            // we found a primitive type, we don't have to iterate over properties
            // as we assume these were not registered
            functions[typeId].to(in, out);
            return;
        }

        if (info->type() == TypeInfo::Type::Class)
        {
            for (auto& property: info->class_().properties)
            {
                std::any propertyIn = property.get(in);
                out[property.name] = nlohmann::json::object();
                nlohmann::json& propertyOut = out[property.name];
                propertyNodeToJson(propertyIn, propertyOut, info->class_(), property.node);
            }
        }
    }

    void JsonSerializer::propertyNodeToJson(std::any in, nlohmann::json& out, ClassInfo& info, size_t nodeIndex)
    {
        PropertyNode& n = info.nodes[nodeIndex];
        switch (n.type)
        {
            case PropertyNode::Type::Type:
            {
                typeToJson(in, out, n.object.typeId);
                break;
            }
            case PropertyNode::Type::List:
            {
                out = nlohmann::json::array(); // convert to array
                size_t size = n.list.size(in);
                for (size_t i = 0; i < size; i++)
                {
                    std::any listIn = n.list.at(in, i);
                    nlohmann::json& listOut = out.emplace_back(nlohmann::json::object());
                    propertyNodeToJson(listIn, listOut, info, n.list.valueNode);
                }
                break;
            }
            case PropertyNode::Type::Dictionary:
            {
                n.dictionary.iterate(in, [&](std::string const& key, std::any dictionaryIn) {
                    // for each dictionary entry, add a json object
                    out[key] = nlohmann::json::object();
                    nlohmann::json& dictionaryOut = out[key];
                    propertyNodeToJson(dictionaryIn, dictionaryOut, info, n.dictionary.valueNode);
                });
                break;
            }
            case PropertyNode::Type::Pointer:
            {
                // this means we could have a polymorphic type, so we need to check the type of the std::any.
                // how do we do this? do we use dynamic_cast?

                break;
            }
        }
    }
}