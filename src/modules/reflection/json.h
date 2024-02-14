//
// Created by Arjo Nagelhout on 14/02/2024.
//

#ifndef SHAPEREALITY_JSON_H
#define SHAPEREALITY_JSON_H

#include <reflection/type_info.h>

namespace reflection
{
    // serialization from and to json using type info

    //-----------------------------------------------------
    // To JSON
    //-----------------------------------------------------

    void objectToJson(TypeInfoRegistry& r, std::any in, nlohmann::json& out, type_id typeId);

    void nodeToJson(TypeInfoRegistry& r, std::any in, nlohmann::json& out, TypeInfo& info, size_t nodeIndex);

    template<typename Type>
    nlohmann::json toJson(TypeInfoRegistry& r, Type& in)
    {
        nlohmann::json out = nlohmann::json::object();
        objectToJson(r, &in, out, TypeIndex<Type>::value());
        return out;
    }

    //-----------------------------------------------------
    // From JSON
    //-----------------------------------------------------

    void objectFromJson(TypeInfoRegistry& r, nlohmann::json const& in, std::any out, type_id typeId);

    void nodeFromJson(TypeInfoRegistry& r, nlohmann::json const& in, std::any out, TypeInfo& info, size_t nodeIndex);

    template<typename Type>
    void fromJson(TypeInfoRegistry& r, nlohmann::json const& in, Type& out)
    {
        objectFromJson(r, in, &out, TypeIndex<Type>::value());
    }

    template<typename Type>
    Type fromJson(TypeInfoRegistry& r, std::string const& in)
    {
        nlohmann::json parsed = nlohmann::json::parse(in);
        Type out;
        fromJson(r, parsed, out);
        return out;
    }
}

#endif //SHAPEREALITY_JSON_H
