#include "graphics/application.h"

#include "math/vector.h"
#include "math/vector.inl"
#include "math/matrix.h"
#include "math/matrix.inl"
#include "math/quaternion.h"
#include "math/quaternion.inl"
#include "math/utils.h"

#include "graphics/input.h"
#include "graphics/window.h"
#include "graphics/graphics.h"
#include "graphics/device.h"
#include "graphics/command_queue.h"
#include "graphics/render_pass.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "graphics/render_pipeline_state.h"
#include "graphics/buffer.h"

#include "entity/registry.h"
#include "entity/components/hierarchy.h"

#include "renderer/mesh.h"
#include "renderer/camera.h"
#include "renderer/shader.h"
#include "renderer/material.h"
#include "renderer/mesh_renderer.h"
#include "renderer/transform.h"

#include "assets/import/gltf_importer.h"
#include "assets/import/texture_importer.h"
#include "imgui.h"
#include "renderer/imgui_implementation.h"

#include <iostream>

using namespace graphics;
using namespace renderer;
using namespace entity;

// factory method to create an object with Hierarchy, Transform and MeshRenderer
void createObject(Registry& r, entity_type index, TransformComponent transformComponent,
                  MeshRendererComponent meshRendererComponent)
{
    r.createEntity(index);
    r.addComponent<HierarchyComponent>(index);
    r.addComponent<TransformComponent>(index, transformComponent);
    r.addComponent<TransformDirtyComponent>(index); // to make sure the transform gets calculated a first time
    r.addComponent<MeshRendererComponent>(index, meshRendererComponent);
}

struct InstanceData
{
    math::Matrix4 localToWorldTransform;
};

// high level implementation of what the app should be doing
class App final : public IApplicationDelegate, public IWindowRenderDelegate, public IWindowInputDelegate
{
public:
    explicit App() = default;

    ~App() = default;

    void onEvent(InputEvent const& event, Window* window) override
    {
        imgui_backend::onEvent(event);

        if (event.type == InputEventType::Keyboard)
        {
            std::cout << event.toString() << std::endl;
            // on macOS, the Meta (Command) key causes the up event to not be called
            // on any keys that were currently down. So we should stop all movement
            if ((event.keyboard.modifiers & KeyboardModifier_Meta) != 0)
            {
                for (int i = 0; i < 6; i++)
                {
                    pressed[i] = 0;
                }
                return;
            }

            int value = -1;
            if (event.keyboard.type == KeyboardEventType::Down)
            {
                value = 1;
            }
            else if (event.keyboard.type == KeyboardEventType::Up)
            {
                value = 0;
            }
            else
            {
                return;
            }

            int index = 0;
            switch (event.keyboard.keyCode)
            {
                //@formatter:off
                case KeyCode::W: index = w; break;
                case KeyCode::A: index = a; break;
                case KeyCode::S: index = s; break;
                case KeyCode::D: index = d; break;
                case KeyCode::Q: index = q; break;
                case KeyCode::E: index = e; break;
                case KeyCode::LeftArrow: index = left; break;
                case KeyCode::RightArrow: index = right; break;
                case KeyCode::UpArrow: index = up; break;
                case KeyCode::DownArrow: index = down; break;
                default: return;
                //@formatter:on
            }

            pressed[index] = value;
        }
    }

    // todo: these assets should only converted once, and then it should simply use a cached version.
    //          so a project has a corresponding cache that contains the converted assets.
    //
    // this importing should eventually be abstracted away so that we can specify which assets to load.
    // on runtime, it should only read the binary formats that don't have to imported.
    void importMeshes(std::filesystem::path const& path)
    {
        // import meshes
        assets::GltfImportDescriptor meshImportDescriptor{

        };

        assets::GltfImportResult importMeshResult = assets::importGltf(pDevice, path, meshImportDescriptor,
                                                                       pMeshes);
        if (!importMeshResult.success)
        {
            exit(1);
        }
    }

    std::unique_ptr<ITexture> importTexture(std::filesystem::path const& path)
    {
        std::unique_ptr<ITexture> outTexture;
        assets::TextureImportDescriptor textureImportDescriptor{

        };
        assets::TextureImportResult importTextureResult = assets::importTexture(pDevice, path,
                                                                                textureImportDescriptor, outTexture);
        if (!importTextureResult.success)
        {
            std::cout << importTextureResult.errorMessage << std::endl;
            exit(1);
        }

        return outTexture;
    }

    void applicationDidFinishLaunching() override
    {
        // command queue
        CommandQueueDescriptor commandQueueDescriptor{};
        pCommandQueue = pDevice->createCommandQueue(commandQueueDescriptor);

        // shader library
        pShaderLibrary = pDevice->createShaderLibrary(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/build/shaders/library.metallib");

        // depth stencil state
        DepthStencilDescriptor depthStencilDescriptor{
            .depthCompareFunction = CompareFunction::LessEqual,
            .depthWriteEnabled = true,
        };
        pDepthStencilState = pDevice->createDepthStencilState(depthStencilDescriptor);

        // camera
        pCamera = std::make_unique<renderer::Camera>(pDevice);

        // meshes
        importMeshes("/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/scene.gltf");

        // shaders
        pShader = std::make_unique<renderer::Shader>(pDevice, pShaderLibrary.get(), "simple_vertex", "simple_fragment");

        // textures
        pTextureBaseColor = importTexture(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/default_baseColor.png");
        pTextureMaterial25 = importTexture(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/11112_sheet_Material__25_baseColor.png");
        pTextureMaterial37 = importTexture(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/11112_sheet_Material__37_baseColor.png");

        // materials
        pMaterialBaseColor = std::make_unique<renderer::Material>(pShader.get(), pTextureBaseColor.get());
        pMaterial25 = std::make_unique<renderer::Material>(pShader.get(), pTextureMaterial25.get());
        pMaterial37 = std::make_unique<renderer::Material>(pShader.get(), pTextureMaterial37.get());

        // create objects
        createObject(r, 0,
                     TransformComponent{
                         .localPosition = math::Vector3{{0, 1.f, 0}},
                         .localRotation = math::Quaternion::identity,
                         .localScale = math::Vector3::create(3.f)
                     },
                     MeshRendererComponent{
                         .pMesh = pMeshes[0].get(),
                         .pMaterial = pMaterial25.get()
                     });
        createObject(r, 1, TransformComponent{}, MeshRendererComponent{pMeshes[1].get(), pMaterial25.get()});
        createObject(r, 2, TransformComponent{}, MeshRendererComponent{pMeshes[2].get(), pMaterial37.get()});
        createObject(r, 3, TransformComponent{}, MeshRendererComponent{pMeshes[3].get(), pMaterial37.get()});
        createObject(r, 4, TransformComponent{}, MeshRendererComponent{pMeshes[4].get(), pMaterialBaseColor.get()});

        // create instance data buffer (this should be resized) eventually this will be moved to the renderer, and
        // not inside main

        graphics::BufferDescriptor instanceDataBufferDescriptor{
            .storageMode = BufferDescriptor::StorageMode::Managed,
            .length = 1 * sizeof(InstanceData), // one object
            .stride = sizeof(InstanceData)
        };
        pInstanceDataBuffer = pDevice->createBuffer(instanceDataBufferDescriptor);

        // create imgui context
        ImGui::CreateContext();
        imgui_backend::init(pDevice, pWindow, pShaderLibrary.get());
    }

    void applicationWillTerminate() override
    {
        imgui_backend::shutdown();
        ImGui::DestroyContext();
    }

    void render(Window* window) override
    {
        //-------------------------------------------------
        // Update camera transform
        //-------------------------------------------------

        // set camera aspect ratio based on the current size of the window
        math::Rect rect = window->getRect();
        pCamera->setAspectRatio(rect.width / rect.height);

        auto const xDir = static_cast<float>(pressed[d] - pressed[a]);
        auto const yDir = static_cast<float>(pressed[e] - pressed[q]);
        auto const zDir = static_cast<float>(pressed[w] - pressed[s]);
        offset += math::Vector3{{xDir, yDir, zDir}} * speed;

        auto const deltaHorizontalRotation = static_cast<float>(pressed[right] - pressed[left]) * rotationSpeed;
        auto const deltaVerticalRotation = static_cast<float>(pressed[up] - pressed[down]) * rotationSpeed;
        horizontalRotation += deltaHorizontalRotation;
        verticalRotation += deltaVerticalRotation;

        math::Quaternion cameraRotation = math::Quaternion::fromEulerInRadians(
            -math::degreesToRadians(verticalRotation), math::degreesToRadians(horizontalRotation), 0
        );

        math::Matrix4 cameraTransform = math::createTranslationRotationScaleMatrix(
            offset, cameraRotation, math::Vector3::one);

        pCamera->setTransform(cameraTransform);

        //-------------------------------------------------
        // Update transforms of objects
        //-------------------------------------------------

        time += 0.005f;
        float yPosition = 20* sin(time);
        float xScale = 1.f + 0.5f * sin(time + 1);
        float yScale = 1.f + 0.5f * sin(time + 2);
        float zScale = 1.f + 0.5f * sin(time + 3);

        // update local position of object 0
        renderer::setLocalPosition(r, 0, math::Vector3{{0, yPosition, 0}});
        renderer::setLocalScale(r, 0, math::Vector3{{xScale, yScale, zScale}});

        // updates the transform components based on the hierarchy
        renderer::computeLocalToWorldMatrices(r);

        // update instance data buffer with updated transforms
        for (auto [entityId, transform]: r.view<TransformComponent>())
        {
            auto* pInstanceData = reinterpret_cast<InstanceData*>(pInstanceDataBuffer->getContents());
            pInstanceData->localToWorldTransform = transform.localToWorldTransform.transpose();
            pInstanceDataBuffer->didModifyRange(
                graphics::Range{
                    .offset = static_cast<unsigned int>(entityId),
                    .length = sizeof(InstanceData)
                });
        }

        //-------------------------------------------------
        // Draw objects with MeshRenderers on the screen
        //-------------------------------------------------

        std::unique_ptr<RenderPassDescriptor> renderPassDescriptor = window->getRenderPassDescriptor();
        std::unique_ptr<ICommandBuffer> cmd = pCommandQueue->getCommandBuffer();

        cmd->beginRenderPass(*renderPassDescriptor);
        cmd->setWindingOrder(WindingOrder::Clockwise);
        cmd->setCullMode(CullMode::Back);
        cmd->setTriangleFillMode(TriangleFillMode::Fill);
        cmd->setDepthStencilState(pDepthStencilState.get());

        // iterate over all mesh renderers
        for (auto [entityId, meshRenderer, transform]: r.view<MeshRendererComponent, TransformComponent>(
            entity::IterationPolicy::UseFirstComponent))
        {
            renderer::Mesh* mesh = meshRenderer.pMesh;
            renderer::Material* material = meshRenderer.pMaterial;

            cmd->setRenderPipelineState(material->getShader()->getRenderPipelineState());

            cmd->setVertexStageBuffer(mesh->getVertexBuffer(), /*offset*/ 0, /*atIndex*/ 0);
            cmd->setVertexStageBuffer(pCamera->getCameraDataBuffer(), /*offset*/ 0, /*atIndex*/ 1);
            cmd->setVertexStageBuffer(pInstanceDataBuffer.get(), /*offset*/ 0, /*atIndex*/ 2);

            cmd->setFragmentStageTexture(material->getTexture(), 0);

            cmd->drawIndexedPrimitives(PrimitiveType::Triangle,
                /*indexCount*/ mesh->getIndexCount(),
                /*indexBuffer*/ mesh->getIndexBuffer(),
                /*indexBufferOffset*/ 0,
                /*instanceCount*/ 1,
                /*baseVertex*/ 0,
                /*baseInstance*/ 0);
        }

        cmd->endRenderPass();

        //-------------------------------------------------
        // Draw ImGui user interface
        //-------------------------------------------------

        // ImGui: New frame
        imgui_backend::newFrame(*renderPassDescriptor);
        ImGui::NewFrame();

        // ImGui: Draw UI
        bool openImGuiDemoWindow = true;
        ImGui::ShowDemoWindow(&openImGuiDemoWindow);

        // ImGui: Render
        ImGui::Render();
        imgui_backend::renderDrawData(ImGui::GetDrawData(), cmd.get());

        //-------------------------------------------------
        // Submit to window
        //-------------------------------------------------

        std::unique_ptr<ITexture> drawable = window->getDrawable();
        cmd->present(drawable.get());
        cmd->commit();
    }

    void setDevice(IDevice* device)
    {
        pDevice = device;
    }

    void setWindow(Window* window)
    {
        pWindow = window;
    }

private:
    IDevice* pDevice{nullptr};
    Window* pWindow{nullptr};

    std::unique_ptr<ICommandQueue> pCommandQueue;
    std::unique_ptr<IShaderLibrary> pShaderLibrary;
    std::unique_ptr<IDepthStencilState> pDepthStencilState;

    std::unique_ptr<renderer::Camera> pCamera;

    std::vector<std::unique_ptr<renderer::Mesh>> pMeshes;

    // textures
    std::unique_ptr<graphics::ITexture> pTextureMaterial25;
    std::unique_ptr<graphics::ITexture> pTextureMaterial37;
    std::unique_ptr<graphics::ITexture> pTextureBaseColor;

    // shaders
    std::unique_ptr<renderer::Shader> pShader;

    // materials
    std::unique_ptr<renderer::Material> pMaterial25;
    std::unique_ptr<renderer::Material> pMaterial37;
    std::unique_ptr<renderer::Material> pMaterialBaseColor;

    // entity
    Registry r;

    // instance data buffer
    std::unique_ptr<graphics::IBuffer> pInstanceDataBuffer;

    // very simple and dumb temporary way to get key input for moving around
    constexpr static int w = 0;
    constexpr static int a = 1;
    constexpr static int s = 2;
    constexpr static int d = 3;
    constexpr static int q = 4;
    constexpr static int e = 5;
    constexpr static int left = 6;
    constexpr static int right = 7;
    constexpr static int up = 8;
    constexpr static int down = 9;
    std::array<int, 10> pressed{};

    float speed = 1.0f;
    float rotationSpeed = 1.0f;

    math::Vector3 offset = math::Vector3::zero;
    float horizontalRotation = 0.f;
    float verticalRotation = 0.0f;

    float time = 0.f;
};

int main(int argc, char* argv[])
{
    // create application, should be done first
    Application application{};

    App app{};
    application.setDelegate(&app);

    GraphicsBackend backend = GraphicsBackend::Metal;
    std::unique_ptr<IDevice> device = createDevice(backend);

    WindowDescriptor descriptor{
        .x = 600,
        .y = 500,
        .width = 500,
        .height = 500,
        .flags = WindowFlags_Default,
        .clearColor = math::Vector4{{0.5f, 1.f, 1.f, 1.f}}
    };
    std::unique_ptr<Window> window = device->createWindow(descriptor);
    window->setTitle("Engine somewhat from scratch");
    window->setMinSize(300, 100);
    window->setRenderDelegate(&app);
    window->setInputDelegate(&app); // app listens to input from the window

    app.setDevice(device.get());
    app.setWindow(window.get());

    // run application
    application.run();

    return 0;
}