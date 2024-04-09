//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "register.h"

#include <entity/entity_registry.h>

#include <reflection/class.h>

namespace entity
{
    REGISTER_REFLECTION
    {
        reflection::register_::Class<EntityRegistry>("EntityRegistry")
            //.property<&EntityRegistry::components>
            .emplace(reflection.types);
    }
}