//
// Created by Arjo Nagelhout on 29/02/2024.
//

#ifndef SHAPEREALITY_EDITOR_H
#define SHAPEREALITY_EDITOR_H

#include <graphics/application.h>

#include <math/vector.h>
#include <math/vector.inl>
#include <math/matrix.h>
#include <math/matrix.inl>
#include <math/quaternion.h>
#include <math/quaternion.inl>
#include <math/utility.h>

#include <graphics/input.h>
#include <graphics/window.h>
#include <graphics/types.h>
#include <graphics/device.h>
#include <graphics/command_queue.h>
#include <graphics/render_pass.h>
#include <graphics/texture.h>
#include <graphics/shader.h>
#include <graphics/render_pipeline_state.h>
#include <graphics/buffer.h>

#include <entity/entity_registry.h>
#include <entity/components/hierarchy.h>

#include <scene/scene.h>

#include <renderer/mesh.h>
#include <renderer/camera.h>
#include <renderer/shader.h>
#include <renderer/material.h>
#include <renderer/mesh_renderer.h>
#include <renderer/transform.h>
#include <renderer/scene_renderer.h>

#include <asset/asset_database.h>
#include <asset/import_registry.h>

#include <input/input.h>

#include <reflection/reflection.h>

#include <BS_thread_pool.hpp>

#include "ui.h"
#include "camera_controller.h"

namespace editor
{
    void createObject(entity::EntityRegistry& r,
                      entity::EntityId entityId,
                      renderer::TransformComponent transformComponent,
                      renderer::MeshRendererComponent meshRendererComponent);

    class Editor final
        : public graphics::IApplicationDelegate,
          public graphics::IWindowRenderDelegate,
          public graphics::IWindowInputDelegate
    {
    public:
        explicit Editor(asset::AssetDatabase& assets);

        ~Editor();

        void onEvent(graphics::InputEvent const& event, graphics::Window* window) override;

        void applicationDidFinishLaunching() override;

        void applicationWillTerminate() override;

        void render(graphics::Window* _window) override;

        graphics::IDevice* device = nullptr;
        graphics::Window* window = nullptr;

    private:
        asset::AssetDatabase& assets;

        // meshes
        asset::Asset mesh0;
        asset::Asset mesh1;
        asset::Asset mesh2;
        asset::Asset mesh3;
        asset::Asset mesh4;
        asset::Asset dummyMesh;

        // textures
        asset::Asset textureBaseColor;
        asset::Asset textureMaterial25;
        asset::Asset textureMaterial37;

        std::unique_ptr<input::Input> input;
        std::unique_ptr<scene::Scene> scene;

        std::unique_ptr<editor::UI> ui;

        std::unique_ptr<graphics::ICommandQueue> commandQueue;
        std::unique_ptr<graphics::IShaderLibrary> shaderLibrary;
        std::unique_ptr<graphics::IDepthStencilState> depthStencilState;

        // camera
        std::unique_ptr<renderer::Camera> camera;
        std::unique_ptr<CameraController> cameraController;

        // shaders
        std::unique_ptr<renderer::Shader> newColorShader;
        std::unique_ptr<renderer::Shader> newShader;

        // materials
        renderer::Material material25;
        renderer::Material material37;
        renderer::Material materialBaseColor;
        renderer::Material newColorMaterial;
    };
}

#endif //SHAPEREALITY_EDITOR_H
