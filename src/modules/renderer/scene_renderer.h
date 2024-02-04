//
// Created by Arjo Nagelhout on 29/01/2024.
//

#ifndef SHAPEREALITY_SCENE_RENDERER_H
#define SHAPEREALITY_SCENE_RENDERER_H

#include "scene/scene.h"

/**
 * @namespace renderer
 * @brief renderer for the renderer-agnostic scene representation, builds upon graphics to render the scene
 */
namespace renderer
{
    // is responsible for creating and managing all
    // resources required to render the scene representation
    class SceneRenderer
    {
    public:
        explicit SceneRenderer();

        ~SceneRenderer();

        void render();

    private:
        scene::Scene* scene;
    };
}

#endif //SHAPEREALITY_SCENE_RENDERER_H
