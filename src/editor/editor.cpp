//
// Created by Arjo Nagelhout on 29/02/2024.
//

#include "editor.h"
#include "asset/import/png.h"

#include <iostream>

#include <common/thread_pool.h>

namespace editor
{
    void createObject(entity::EntityRegistry& r,
                      entity::Entity index,
                      renderer::TransformComponent transformComponent,
                      renderer::MeshRendererComponent meshRendererComponent)
    {
        r.createEntity(index);
        r.addComponent<entity::HierarchyComponent>(index);
        r.addComponent<renderer::VisibleComponent>(index);
        r.addComponent<renderer::TransformComponent>(index, transformComponent);
        r.addComponent<renderer::TransformDirtyComponent>(index); // to make sure the transform gets calculated on start
        r.addComponent<renderer::MeshRendererComponent>(index, meshRendererComponent);
    }

    Editor::Editor(std::filesystem::path const& inputDirectory,
                   std::filesystem::path const& loadDirectory)
        : assets(common::ThreadPool::shared(),
                 reflection::JsonSerializer::shared(),
                 importers,
                 inputDirectory,
                 loadDirectory,
                 /* use cache */ false)
    {
        importers.emplace(asset::importPng, {"png"});
    }

    Editor::~Editor() = default;

    void Editor::onEvent(graphics::InputEvent const& event, graphics::Window* _window)
    {
        input->onEvent(event);
        ui->onEvent(event);
    }

    void Editor::importMeshes(std::filesystem::path const& path)
    {
        // import meshes
        asset::GltfImportDescriptor meshImportDescriptor{

        };

        asset::GltfImportResult importMeshResult = asset::importGltf(device, path, meshImportDescriptor,
                                                                     meshes);
        if (!importMeshResult.success)
        {
            exit(1);
        }
    }

    std::unique_ptr<graphics::ITexture> Editor::importTexture(std::filesystem::path const& path)
    {
        std::unique_ptr<graphics::ITexture> outTexture;
        asset::TextureImportDescriptor textureImportDescriptor{

        };
        asset::TextureImportResult importTextureResult = asset::importTexture(device, path,
                                                                              textureImportDescriptor, outTexture);
        if (!importTextureResult.success)
        {
            std::cout << importTextureResult.errorMessage << std::endl;
            exit(1);
        }

        return outTexture;
    }

    void Editor::applicationDidFinishLaunching()
    {
        // import textures
        assets.importFile("models/sea_house/textures/default_baseColor.png");
        assets.importFile("models/sea_house/textures/11112_sheet_Material__25_baseColor.png");
        assets.importFile("models/sea_house/textures/11112_sheet_Material__37_baseColor.png");

        // command queue
        graphics::CommandQueueDescriptor commandQueueDescriptor{};
        commandQueue = device->createCommandQueue(commandQueueDescriptor);

        // shader library
        shaderLibrary = device->createShaderLibrary(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/build/shaders/library.metallib");

        // depth stencil state
        graphics::DepthStencilDescriptor depthStencilDescriptor{
            .depthCompareFunction = graphics::CompareFunction::LessEqual,
            .depthWriteEnabled = true,
        };
        depthStencilState = device->createDepthStencilState(depthStencilDescriptor);

        // camera
        camera = std::make_unique<renderer::Camera>(device);

        // meshes
        importMeshes("/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/scene.gltf");

        // shaders
        shader = std::make_unique<renderer::Shader>(device, shaderLibrary.get(), "simple_vertex",
                                                    "simple_fragment");

        // textures
        textureBaseColor = importTexture(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/default_baseColor.png");
        textureMaterial25 = importTexture(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/11112_sheet_Material__25_baseColor.png");
        textureMaterial37 = importTexture(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/11112_sheet_Material__37_baseColor.png");

        // materials
        materialBaseColor = std::make_unique<renderer::Material>(shader.get(), textureBaseColor.get());
        material25 = std::make_unique<renderer::Material>(shader.get(), textureMaterial25.get());
        material37 = std::make_unique<renderer::Material>(shader.get(), textureMaterial37.get());

        // scene
        scene = std::make_unique<scene::Scene>();

        // create objects
        createObject(scene->entities, 0,
                     renderer::TransformComponent{
                         .localPosition = math::Vector3{{0, 1.f, 0}},
                         .localRotation = math::Quaternion::identity,
                         .localScale = math::Vector3::create(3.f)
                     },
                     renderer::MeshRendererComponent{
                         .mesh = meshes[0].get(),
                         .material = material25.get()
                     });
        createObject(scene->entities, 1, renderer::TransformComponent{},
                     renderer::MeshRendererComponent{meshes[1].get(), material25.get()});
        createObject(scene->entities, 2, renderer::TransformComponent{},
                     renderer::MeshRendererComponent{meshes[2].get(), material37.get()});
        createObject(scene->entities, 4, renderer::TransformComponent{},
                     renderer::MeshRendererComponent{meshes[4].get(), materialBaseColor.get()});
        createObject(scene->entities, 3, renderer::TransformComponent{},
                     renderer::MeshRendererComponent{meshes[3].get(), material37.get()});

        // editor UI
        ui = std::make_unique<editor::UI>(device, window, shaderLibrary.get());
        ui->setRegistry(&scene->entities);

        // input handler
        input = std::make_unique<input::Input>();
    }

    void Editor::applicationWillTerminate()
    {
        ui.reset();
    }

    void Editor::render(graphics::Window* _window)
    {
        std::unique_ptr<graphics::RenderPassDescriptor> renderPassDescriptor = _window->getRenderPassDescriptor();

        ui->update(
            *renderPassDescriptor); // todo: move into one render function that takes a scene render function as an argument

        //-------------------------------------------------
        // Update camera transform
        //-------------------------------------------------

        // only update if UI didn't capture keyboard input
        if (!ui->getCapturedKeyboard())
        {
            // set camera aspect ratio based on the current size of the window
            graphics::Size size = _window->getContentViewSize();
            camera->setAspectRatio(size.width / size.height);

            auto const xDir = static_cast<float>(input->getKey(graphics::KeyCode::D) -
                                                 input->getKey(graphics::KeyCode::A));
            auto const yDir = static_cast<float>(input->getKey(graphics::KeyCode::E) -
                                                 input->getKey(graphics::KeyCode::Q));
            auto const zDir = static_cast<float>(input->getKey(graphics::KeyCode::W) -
                                                 input->getKey(graphics::KeyCode::S));
            offset += math::Vector3{{xDir, yDir, zDir}} * speed;

            auto const deltaHorizontalRotation =
                static_cast<float>(input->getKey(graphics::KeyCode::RightArrow) -
                                   input->getKey(graphics::KeyCode::LeftArrow)) *
                rotationSpeed;
            auto const deltaVerticalRotation =
                static_cast<float>(input->getKey(graphics::KeyCode::UpArrow) -
                                   input->getKey(graphics::KeyCode::DownArrow)) *
                rotationSpeed;
            horizontalRotation += deltaHorizontalRotation;
            verticalRotation += deltaVerticalRotation;

            math::Quaternion cameraRotation = math::Quaternion::createFromEulerInRadians(
                math::Vector3{
                    {-math::degreesToRadians(verticalRotation), math::degreesToRadians(horizontalRotation), 0}
                }
            );

            math::Matrix4 cameraTransform = math::createTranslationRotationScaleMatrix(
                offset, cameraRotation, math::Vector3::one
            );

            camera->setTransform(cameraTransform);
        }

        //-------------------------------------------------
        // Update transforms of all objects (should be refactored into renderer / scene abstraction)
        //-------------------------------------------------

        // updates the transform components based on the hierarchy
        renderer::computeLocalToWorldMatrices(scene->entities);

        //-------------------------------------------------
        // Draw objects with MeshRenderers on the screen (should be refactored into renderer / scene abstraction)
        //-------------------------------------------------

        std::unique_ptr<graphics::ICommandBuffer> cmd = commandQueue->getCommandBuffer();

        cmd->beginRenderPass(*renderPassDescriptor);
        cmd->setWindingOrder(graphics::WindingOrder::Clockwise);
        cmd->setCullMode(graphics::CullMode::Back);
        cmd->setTriangleFillMode(graphics::TriangleFillMode::Fill);
        cmd->setDepthStencilState(depthStencilState.get());

        for (auto [entityId, meshRenderer, transform, visible]:
            scene->entities.view<renderer::MeshRendererComponent, renderer::TransformComponent, renderer::VisibleComponent>(
                entity::IterationPolicy::UseFirstComponent))
        {
            renderer::Mesh* mesh = meshRenderer.mesh;
            renderer::Material* material = meshRenderer.material;

            cmd->setRenderPipelineState(material->getShader()->getRenderPipelineState());

            cmd->setVertexStageBuffer(mesh->getVertexBuffer(), /*offset*/ 0, /*atIndex*/ 0);
            cmd->setVertexStageBuffer(camera->getCameraDataBuffer(), /*offset*/ 0, /*atIndex*/ 1);

            // set small constant data that is different for each object
            math::Matrix4 localToWorldTransform = transform.localToWorldTransform.transpose();
            cmd->setVertexStageBytes(static_cast<void const*>(&localToWorldTransform),
                /*length*/ sizeof(math::Matrix4),
                /*atIndex*/ 2);

            cmd->setFragmentStageTexture(material->getTexture(), 0);

            cmd->drawIndexedPrimitives(graphics::PrimitiveType::Triangle,
                /*indexCount*/ mesh->getIndexCount(),
                /*indexBuffer*/ mesh->getIndexBuffer(),
                /*indexBufferOffset*/ 0,
                /*instanceCount*/ 1,
                /*baseVertex*/ 0,
                /*baseInstance*/ 0);
        }

        //-------------------------------------------------
        // Draw ImGui user interface
        //-------------------------------------------------

        ui->render(cmd.get());

        cmd->endRenderPass();

        //-------------------------------------------------
        // Submit to window
        //-------------------------------------------------

        std::unique_ptr<graphics::ITexture> drawable = window->getDrawable();
        cmd->present(drawable.get());
        cmd->commit();
    }
}