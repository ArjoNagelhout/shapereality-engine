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
}

#endif //SHAPEREALITY_JSON_H
