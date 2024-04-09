//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "register.h"

#include <reflection/class.h>
#include <scene/scene.h>
#include <reflection/type_registry.h>

namespace scene
{
    REGISTER_REFLECTION
    {
        reflection::register_::Class<Scene>("Scene")
            .member<&Scene::name>("name")
            .member<&Scene::entities>("entities")
            .emplace(reflection.types);
    }
}