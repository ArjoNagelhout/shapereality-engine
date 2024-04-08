//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "reflection.h"

#include <reflection/class.h>
#include <scene/scene.h>

namespace scene
{
    void registerReflection()
    {
        reflection::ClassInfoBuilder<Scene>("Scene")
            .member<&Scene::name>("name")
            .member<&Scene::entities>("entities")
            .emplace();
    }
}