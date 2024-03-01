//
// Created by Arjo Nagelhout on 15/02/2024.
//

#include "type_info.h"

#include <cassert>

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

    bool TypeInfoRegistry::contains(TypeId typeId) const
    {
        return types.contains(typeId);
    }

    TypeInfo* TypeInfoRegistry::get(TypeId typeId)
    {
        if (types.contains(typeId))
        {
            return &(types.at(typeId));
        }
        else
        {
            return nullptr;
        }
    }
}