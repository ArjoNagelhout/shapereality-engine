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
#include <math/utils.h>

#include <graphics/input.h>
#include <graphics/window.h>
#include <graphics/graphics.h>
#include <graphics/device.h>
#include <graphics/command_queue.h>
#include <graphics/render_pass.h>
#include <graphics/texture.h>
#include <graphics/shader.h>
#include <graphics/render_pipeline_state.h>
#include <graphics/buffer.h>

#include <entity/registry.h>
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

#include <asset/import/gltf.h>
#include <asset/import/texture.h>

#include <input/input.h>

#include "ui.h"

namespace fs = std::filesystem;

namespace editor
{
    void createObject(entity::Registry& r,
                      entity::Entity index,
                      renderer::TransformComponent transformComponent,
                      renderer::MeshRendererComponent meshRendererComponent);

    class Editor final
        : public graphics::IApplicationDelegate,
          public graphics::IWindowRenderDelegate,
          public graphics::IWindowInputDelegate
    {
    public:
        explicit Editor();

        ~Editor();

        void onEvent(graphics::InputEvent const& event, graphics::Window* window) override;

        void importMeshes(fs::path const& absolutePath);

        [[nodiscard]] std::unique_ptr<graphics::ITexture> importTexture(fs::path const& absolutePath);

        void applicationDidFinishLaunching() override;

        void applicationWillTerminate() override;

        void render(graphics::Window* _window) override;

        graphics::IDevice* device{nullptr};
        graphics::Window* window{nullptr};

    private:
        std::unique_ptr<input::Input> input;
        std::unique_ptr<scene::Scene> scene;
        entity::Registry* r{nullptr};

        std::unique_ptr<editor::UI> ui;

        std::unique_ptr<asset::ImportRegistry> importers;
        std::unique_ptr<asset::AssetDatabase> assets;

        std::unique_ptr<graphics::ICommandQueue> commandQueue;
        std::unique_ptr<graphics::IShaderLibrary> shaderLibrary;
        std::unique_ptr<graphics::IDepthStencilState> depthStencilState;

        std::unique_ptr<renderer::Camera> camera;

        std::vector<std::unique_ptr<renderer::Mesh>> meshes;

        // textures
        std::unique_ptr<graphics::ITexture> textureMaterial25;
        std::unique_ptr<graphics::ITexture> textureMaterial37;
        std::unique_ptr<graphics::ITexture> textureBaseColor;

        // shaders
        std::unique_ptr<renderer::Shader> shader;

        // materials
        std::unique_ptr<renderer::Material> material25;
        std::unique_ptr<renderer::Material> material37;
        std::unique_ptr<renderer::Material> materialBaseColor;

        float speed = 1.0f;
        float rotationSpeed = 1.0f;

        math::Vector3 offset = math::Vector3::zero;
        float horizontalRotation = 0.f;
        float verticalRotation = 0.0f;
    };
}

#endif //SHAPEREALITY_EDITOR_H
