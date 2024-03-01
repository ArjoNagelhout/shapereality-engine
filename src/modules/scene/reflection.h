//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_SCENE_REFLECTION_H
#define SHAPEREALITY_SCENE_REFLECTION_H

#include <reflection/type_info.h>
#include <reflection/serialize/json.h>

namespace scene
{
    void registerReflection(reflection::TypeInfoRegistry& r, reflection::JsonSerializer& jsonSerializer);
}

#endif //SHAPEREALITY_SCENE_REFLECTION_H
