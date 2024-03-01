//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "reflection.h"

#include <entity/registry.h>

#include <reflection/type_info.h>

namespace entity
{
    struct Base
    {
        float something = 1.5f;
    };

    struct Inherited : public Base
    {

    };

    void registerReflection()
    {
        reflection::TypeInfoBuilder<Inherited>("Inherited")
            .property<&Inherited::something>("something")
            .emplace();

        reflection::TypeInfoBuilder<EntityRegistry>("EntityRegistry")
            //.property<&EntityRegistry::components>
            .emplace();
    }
}