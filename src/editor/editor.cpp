//
// Created by Arjo Nagelhout on 29/02/2024.
//

#include "editor.h"
#include <import/texture/png.h>

#include <iostream>

#include <common/thread_pool.h>
#include <common/logger.h>
#include <reflection/enum.h>

#include <import/gltf/register.h>

namespace editor
{
    struct MeshRendererNew
    {
        asset::Asset mesh;
        renderer::Material* material;
    };

    void createObject(entity::EntityRegistry& r,
                      entity::EntityId entityId,
                      renderer::TransformComponent transformComponent,
                      renderer::MeshRendererComponent meshRendererComponent)
    {
        r.createEntity(entityId);
        r.addComponent<entity::HierarchyComponent>(entityId);
        r.addComponent<renderer::VisibleComponent>(entityId);
        r.addComponent<renderer::TransformComponent>(entityId, transformComponent);
        r.addComponent<renderer::TransformDirtyComponent>(entityId); // to make sure the transform gets calculated on start
        r.addComponent<renderer::MeshRendererComponent>(entityId, meshRendererComponent);
    }

    void createObjectNew(entity::EntityRegistry& r, entity::EntityId entityId,
                         MeshRendererNew const& meshRenderer)
    {
        r.createEntity(entityId);
        r.addComponent<entity::HierarchyComponent>(entityId);
        r.addComponent<renderer::VisibleComponent>(entityId);
        r.addComponent<renderer::TransformComponent>(entityId);
        r.addComponent<renderer::TransformDirtyComponent>(entityId); // to make sure the transform gets calculated on start
        r.addComponent<MeshRendererNew>(entityId, meshRenderer);
    }

    Editor::Editor(asset::AssetDatabase& assets_) : assets(assets_) {}

    Editor::~Editor() = default;

    void Editor::onEvent(graphics::InputEvent const& event, graphics::Window* _window)
    {
        input->onEvent(event);
        ui->onEvent(event);
    }

    void Editor::importMeshes(std::filesystem::path const& path)
    {
        // import meshes
        import_::gltf::GltfImportDescriptor meshImportDescriptor{

        };

        import_::gltf::GltfImportResult importMeshResult = import_::gltf::importGltf(device, path, meshImportDescriptor, meshes);
        if (!importMeshResult.success)
        {
            exit(1);
        }
    }

    std::unique_ptr<graphics::ITexture> Editor::importTexture(std::filesystem::path const& path)
    {
        std::unique_ptr<graphics::ITexture> outTexture;
        import_::texture::TextureImportDescriptor textureImportDescriptor{

        };
        import_::texture::TextureImportResult importTextureResult = import_::texture::importTexture(
            device, path, textureImportDescriptor, outTexture);

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
        //assets.importFile("models/sea_house/scene.gltf");

        mesh0 = assets.get(asset::AssetId{"models/sea_house/scene.gltf", "Object_0_0.mesh"});
        mesh1 = assets.get(asset::AssetId{"models/sea_house/scene.gltf", "Object_1_0.mesh"});
        mesh2 = assets.get(asset::AssetId{"models/sea_house/scene.gltf", "Object_2_0.mesh"});
        mesh3 = assets.get(asset::AssetId{"models/sea_house/scene.gltf", "Object_3_0.mesh"});
        mesh4 = assets.get(asset::AssetId{"models/sea_house/scene.gltf", "Object_4_0.mesh"});

        renderer::MeshDescriptor descriptor{
            .primitiveType = graphics::PrimitiveType::Triangle,
            .attributes{
                renderer::VertexAttributeDescriptor{
                    .index = 0,
                    .type = renderer::VertexAttribute_Position,
                    .elementType = renderer::ElementType::Vector3,
                    .componentType = renderer::ComponentType::Float
                },
                renderer::VertexAttributeDescriptor{
                    .index = 0,
                    .type = renderer::VertexAttribute_Color,
                    .elementType = renderer::ElementType::Vector3,
                    .componentType = renderer::ComponentType::Float
                }
            },
            .vertexCount = 3,
            .hasIndexBuffer = true,
            .indexCount = 3,
            .indexType = renderer::ComponentType::UnsignedInt,
            .writable = true,
        };
        dummyMesh = asset::makeAsset<renderer::Mesh_>(asset::AssetId{"test", "test.mesh"}, device, descriptor);
        {
            auto& mesh = dummyMesh->get<renderer::Mesh_>();
            std::vector<unsigned int> indices{
                0, 1, 2
            };
            mesh.setIndexData(indices.data());
            std::vector<math::Vector3> positions{
                math::Vector3{{0.5f, -0.5f, 0.0f}},
                math::Vector3{{-0.5f, -0.5f, 0.0f}},
                math::Vector3{{0.0f, 0.5f, 0.0f}},
            };
            mesh.setAttributeData(renderer::VertexAttribute_Position, positions.data(), 0);
            std::vector<math::Vector3> colors{
                math::Vector3{{1.0f, 0.0f, 0.0f}},
                math::Vector3{{0.0f, 0.0f, 1.0f}},
                math::Vector3{{1.0f, 1.0f, 0.0f}},
            };
            mesh.setAttributeData(renderer::VertexAttribute_Color, colors.data(), 0);
        }

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
        //importMeshes("/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/scene.gltf");

        // shaders
        shader = std::make_unique<renderer::Shader>(device, shaderLibrary.get(), "simple_vertex", "simple_fragment");
        newShader = std::make_unique<renderer::Shader>(device, shaderLibrary.get(), "new_vertex", "new_fragment");

        // textures
//        textureBaseColor = importTexture(
//            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/default_baseColor.png");
//        textureMaterial25 = importTexture(
//            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/11112_sheet_Material__25_baseColor.png");
        textureMaterial37 = importTexture(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/11112_sheet_Material__37_baseColor.png");

        // materials
//        materialBaseColor = {shader.get(), textureBaseColor.get()};
//        material25 = {shader.get(), textureMaterial25.get()};
//        material37 = {shader.get(), textureMaterial37.get()};
        newMaterial = {newShader.get(), textureMaterial37.get()};

        // scene
        scene = std::make_unique<scene::Scene>();

        // create objects
//        createObject(
//            scene->entities,
//            0,
//            renderer::TransformComponent{
//                .localPosition = math::Vector3{{0, 1.f, 0}},
//                .localRotation = math::Quaternion::identity,
//                .localScale = math::Vector3::create(3.f)
//            },
//            renderer::MeshRendererComponent{
//                .mesh = meshes[0].get(),
//                .material = &material25
//            }
//        );
//        createObject(scene->entities, 1, renderer::TransformComponent{}, renderer::MeshRendererComponent{meshes[1].get(), &material25});
//        createObject(scene->entities, 2, renderer::TransformComponent{}, renderer::MeshRendererComponent{meshes[2].get(), &material37});
//        createObject(scene->entities, 3, renderer::TransformComponent{}, renderer::MeshRendererComponent{meshes[3].get(), &material37});
//        createObject(scene->entities, 4, renderer::TransformComponent{}, renderer::MeshRendererComponent{meshes[4].get(), &materialBaseColor});
//        createObjectNew(scene->entities, 0, MeshRendererNew{mesh0, &newMaterial});
//        createObjectNew(scene->entities, 1, MeshRendererNew{mesh1, &newMaterial});
//        createObjectNew(scene->entities, 2, MeshRendererNew{mesh2, &newMaterial});
//        createObjectNew(scene->entities, 3, MeshRendererNew{mesh3, &newMaterial});
//        createObjectNew(scene->entities, 4, MeshRendererNew{mesh4, &newMaterial});
        createObjectNew(scene->entities, 5, MeshRendererNew{dummyMesh, &newMaterial});

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

        // todo: move into one render function that takes a scene render function as an argument
        ui->update(*renderPassDescriptor);

        //-------------------------------------------------
        // Update camera transform
        //-------------------------------------------------

        // only update if UI didn't capture keyboard input
        if (!ui->getCapturedKeyboard())
        {
            // set camera aspect ratio based on the current size of the window
            graphics::Size size = _window->getContentViewSize();
            camera->setAspectRatio(size.width / size.height);

            auto const xDir = static_cast<float>(input->getKey(graphics::KeyCode::D) - input->getKey(graphics::KeyCode::A));
            auto const yDir = static_cast<float>(input->getKey(graphics::KeyCode::E) - input->getKey(graphics::KeyCode::Q));
            auto const zDir = static_cast<float>(input->getKey(graphics::KeyCode::W) - input->getKey(graphics::KeyCode::S));
            offset += math::Vector3{{xDir, yDir, zDir}} * speed;

            auto const deltaHorizontalRotation =
                static_cast<float>(input->getKey(graphics::KeyCode::RightArrow) - input->getKey(graphics::KeyCode::LeftArrow)) * rotationSpeed;
            auto const deltaVerticalRotation =
                static_cast<float>(input->getKey(graphics::KeyCode::UpArrow) - input->getKey(graphics::KeyCode::DownArrow)) * rotationSpeed;
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
        cmd->setCullMode(graphics::CullMode::None);
        cmd->setTriangleFillMode(graphics::TriangleFillMode::Fill);
        cmd->setDepthStencilState(depthStencilState.get());

//        for (auto [entityId, meshRenderer, transform, visible]:
//            scene->entities.view<renderer::MeshRendererComponent, renderer::TransformComponent, renderer::VisibleComponent>(
//                entity::IterationPolicy::UseFirstComponent))
//        {
//            renderer::Mesh* mesh = meshRenderer.mesh;
//            renderer::Material* material = meshRenderer.material;
//
//            cmd->setRenderPipelineState(material->shader->getRenderPipelineState());
//
//            cmd->setVertexStageBuffer(mesh->getVertexBuffer(), /*offset*/ 0, /*atIndex*/ 0);
//            cmd->setVertexStageBuffer(camera->getCameraDataBuffer(), /*offset*/ 0, /*atIndex*/ 1);
//
//            // set small constant data that is different for each object
//            math::Matrix4 localToWorldTransform = transform.localToWorldTransform.transpose();
//            cmd->setVertexStageBytes(static_cast<void const*>(&localToWorldTransform),
//                /*length*/ sizeof(math::Matrix4),
//                /*atIndex*/ 2);
//
//            cmd->setFragmentStageTexture(material->texture, 0);
//
//            cmd->drawIndexedPrimitives(graphics::PrimitiveType::Triangle,
//                /*indexCount*/ mesh->getIndexCount(),
//                /*indexBuffer*/ mesh->getIndexBuffer(),
//                /*indexBufferOffset*/ 0,
//                /*instanceCount*/ 1,
//                /*baseVertex*/ 0,
//                /*baseInstance*/ 0);
//        }

        //-------------------------------------------------
        // New mesh asset
        //-------------------------------------------------

        for (auto [entityId, meshRenderer, transform, visible]:
            scene->entities.view<MeshRendererNew, renderer::TransformComponent, renderer::VisibleComponent>(
                entity::IterationPolicy::UseFirstComponent))
        {
            if (!meshRenderer.mesh->success() || !meshRenderer.mesh->valid<renderer::Mesh_>())
            {
                continue;
            }

            auto& mesh = meshRenderer.mesh->get<renderer::Mesh_>();

            renderer::Material* material = meshRenderer.material;
            cmd->setRenderPipelineState(material->shader->getRenderPipelineState());

            // bind vertex attributes
            for (auto& attribute: mesh)
            {
                cmd->setVertexStageBuffer(mesh.vertexBuffer(), /*offset*/ attribute.offset, /*atIndex*/ attribute.index);
                common::log::infoDebug("bound {} to index {}", reflection::enumToString(attribute.descriptor->type), attribute.index);
            }

            // bind one after the last vertex attribute
            cmd->setVertexStageBuffer(
                camera->getCameraDataBuffer(),
                0, /*offset*/
                3); /*atIndex*/

            // set small constant data that is different for each object
            math::Matrix4 localToWorldTransform = transform.localToWorldTransform.transpose();
            cmd->setVertexStageBytes(
                static_cast<void const*>(&localToWorldTransform),
                sizeof(math::Matrix4), /*length*/
                4); /*atIndex*/

            cmd->setFragmentStageTexture(material->texture, 0);

            if (mesh.descriptor().hasIndexBuffer)
            {
                common::log::infoDebug(
                    "indexCount: {}, primitiveType: {}",
                    mesh.descriptor().indexCount,
                    reflection::enumToString(mesh.descriptor().primitiveType));
                // draw with index buffer
                cmd->drawIndexedPrimitives(
                    mesh.descriptor().primitiveType,
                    mesh.descriptor().indexCount, /*indexCount*/
                    mesh.indexBuffer(), /*indexBuffer*/
                    0, /*indexBufferOffset*/
                    1, /*instanceCount*/
                    0, /*baseVertex*/
                    0); /*baseInstance*/
            }
            else
            {
                cmd->drawPrimitives(
                    mesh.descriptor().primitiveType,
                    0, // vertex start
                    mesh.descriptor().vertexCount);
            }

            std::cout << "rendered new object" << std::endl;
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