//
// Created by Arjo Nagelhout on 15/02/2024.
//

#include "type_info.h"

namespace reflection
{
    TypeInfoRegistry::TypeInfoRegistry()
    {
        // built-in types
        emplace<bool>({.name = "bool"});
        emplace<int>({.name = "int"});
        emplace<float>({.name = "float"});
        emplace<double>({.name = "double"});
        emplace<std::string>({.name = "string"});
    }

    bool TypeInfoRegistry::contains(type_id typeId) const
    {
        return types.contains(typeId);
    }

    TypeInfo* TypeInfoRegistry::get(type_id typeId)
    {
        return &(types.at(typeId));
    }
}