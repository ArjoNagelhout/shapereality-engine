//
// Created by Arjo Nagelhout on 29/01/2024.
//

#ifndef SHAPEREALITY_SCENE_H
#define SHAPEREALITY_SCENE_H

#include "entity/registry.h"

namespace renderer
{
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

        // renders the scene todo: make it easy to render the scene to a specific render target
        void render();

    private:
        entity::Registry r;
    };
}

#endif //SHAPEREALITY_SCENE_H
