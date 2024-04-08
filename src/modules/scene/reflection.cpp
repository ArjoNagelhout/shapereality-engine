//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "reflection.h"

#include <reflection/class_info.h>
#include <scene/scene.h>

namespace scene
{
    void registerReflection()
    {
        reflection::ClassInfoBuilder<Scene>("Scene")
            .property<&Scene::name>("name")
            .property<&Scene::entities>("entities")
            .emplace();
    }
}