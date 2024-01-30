//
// Created by Arjo Nagelhout on 29/01/2024.
//

#ifndef SHAPEREALITY_SCENE_RENDERER_H
#define SHAPEREALITY_SCENE_RENDERER_H

#include "scene/scene.h"

namespace rendering
{
    // is responsible for creating and managing all
    // resources required to render the scene representation
    //
    // depends on the `scene` and `assets` module
    //
    // call stack:
    // Editor::render() contains a reference to the scene::Scene to render, calls
    // Scenerendering::render(scene::Scene* scene)
    //
    // let's not try to figure this out now, first write the code, then the abstraction
    //
    class SceneRenderer
    {
    public:
        explicit SceneRenderer();

        ~SceneRenderer();

        // renders a given scene, loads resources
        // when they're needed
        void render(scene::Scene* scene);

    private:

    };
}

#endif //SHAPEREALITY_SCENE_RENDERER_H
