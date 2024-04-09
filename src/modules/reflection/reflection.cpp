//
// Created by Arjo Nagelhout on 09/04/2024.
//

#include "reflection.h"

namespace reflection
{
    Reflection& Reflection::shared()
    {
        static Reflection instance;
        return instance;
    }

    Reflection::Reflection() : types(), json(types), binary(types)
    {

    }
}