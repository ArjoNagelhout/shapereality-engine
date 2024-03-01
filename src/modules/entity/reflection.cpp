//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "reflection.h"

#include <entity/registry.h>

namespace entity
{
    struct Base
    {
        float something = 1.5f;
    };

    struct Inherited : public Base
    {

    };

    void registerReflection(reflection::TypeInfoRegistry& r, reflection::JsonSerializer& jsonSerializer)
    {
        reflection::TypeInfoBuilder<Inherited>("Inherited")
            .property<&Inherited::something>("something")
            .emplace(r);

        reflection::TypeInfoBuilder<EntityRegistry>("EntityRegistry")
            //.property<&EntityRegistry::components>
            .emplace(r);
    }
}