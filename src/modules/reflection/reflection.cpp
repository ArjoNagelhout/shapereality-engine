//
// Created by Arjo Nagelhout on 04/02/2024.
//

#include "reflection.h"

namespace reflection
{
    TypeInfo TypeInfoBuilder::build() const
    {
        return typeInfo;
    }
}