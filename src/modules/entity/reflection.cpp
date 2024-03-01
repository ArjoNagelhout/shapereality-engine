//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "reflection.h"

#include <entity/registry.h>

namespace entity
{
    void registerReflection(reflection::TypeInfoRegistry& r, reflection::JsonSerializer& jsonSerializer)
    {
        reflection::TypeInfoBuilder<EntityRegistry>("EntityRegistry")
            //.property<&EntityRegistry::components>
            .emplace(r);
    }
}