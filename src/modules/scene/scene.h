//
// Created by Arjo Nagelhout on 30/01/2024.
//

#ifndef SHAPEREALITY_SCENE_H
#define SHAPEREALITY_SCENE_H

#include "entity/registry.h"

namespace scene
{
    // representation of the scene and its entities

    // can be rendered either via our own backend (i.e. renderer) or
    // an external one such as RealityKit. This is to enable support for
    // augmented reality applications on visionOS.
    // https://developer.apple.com/visionos/

    struct VisibleComponent
    {
    };

    /**
     * Each `Scene` contains its own registry of entities
     */
    class Scene
    {
    public:
        explicit Scene();

        ~Scene();

    private:
        entity::Registry r;
    };
}

#endif //SHAPEREALITY_SCENE_H
