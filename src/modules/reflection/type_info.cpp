//
// Created by Arjo Nagelhout on 04/02/2024.
//

#include "type_info.h"

namespace reflection
{
    TypeInfoBuilder::TypeInfoBuilder(std::string name)
    {
        typeInfo.name = std::move(name);
    }

    TypeInfo TypeInfoBuilder::build() const
    {
        return typeInfo;
    }
}