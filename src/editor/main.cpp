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

#include "scene/scene.h"

#include "rendering/mesh.h"
#include "rendering/camera.h"
#include "rendering/shader.h"
#include "rendering/material.h"
#include "rendering/mesh_renderer.h"
#include "rendering/transform.h"
#include "rendering/scene_renderer.h"

#include "assets/import/gltf_importer.h"
#include "assets/import/texture_importer.h"

#include "imgui.h"
#include "rendering/imgui/imgui_backend.h"
#include "misc/cpp/imgui_stdlib.h" // for std::string support for ImGui::InputText
#include "ui.h"
#include "input/input.h"

#include <iostream>

using namespace graphics;
using namespace rendering;
using namespace entity;
using namespace scene;

// factory method to create an object with Hierarchy, Transform and MeshRenderer
void createObject(Registry& r, entity_type index, TransformComponent transformComponent,
                  MeshRendererComponent meshRendererComponent)
{
    r.createEntity(index);
    r.addComponent<HierarchyComponent>(index);
    r.addComponent<VisibleComponent>(index);
    r.addComponent<TransformComponent>(index, transformComponent);
    r.addComponent<TransformDirtyComponent>(index); // to make sure the transform gets calculated a first time
    r.addComponent<MeshRendererComponent>(index, meshRendererComponent);
}

// high level implementation of what the app should be doing
class Editor final : public IApplicationDelegate, public IWindowRenderDelegate, public IWindowInputDelegate
{
public:
    explicit Editor() = default;

    ~Editor() = default;

    void onEvent(InputEvent const& event, Window* _window) override
    {
        input->onEvent(event);
        ui->onEvent(event);
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

        assets::GltfImportResult importMeshResult = assets::importGltf(device, path, meshImportDescriptor,
                                                                       meshes);
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
        assets::TextureImportResult importTextureResult = assets::importTexture(device, path,
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
        commandQueue = device->createCommandQueue(commandQueueDescriptor);

        // shader library
        shaderLibrary = device->createShaderLibrary(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/build/shaders/library.metallib");

        // depth stencil state
        DepthStencilDescriptor depthStencilDescriptor{
            .depthCompareFunction = CompareFunction::LessEqual,
            .depthWriteEnabled = true,
        };
        depthStencilState = device->createDepthStencilState(depthStencilDescriptor);

        // camera
        camera = std::make_unique<rendering::Camera>(device);

        // meshes
        importMeshes("/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/scene.gltf");

        // shaders
        shader = std::make_unique<rendering::Shader>(device, shaderLibrary.get(), "simple_vertex", "simple_fragment");

        // textures
        textureBaseColor = importTexture(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/default_baseColor.png");
        textureMaterial25 = importTexture(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/11112_sheet_Material__25_baseColor.png");
        textureMaterial37 = importTexture(
            "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/11112_sheet_Material__37_baseColor.png");

        // materials
        materialBaseColor = std::make_unique<rendering::Material>(shader.get(), textureBaseColor.get());
        material25 = std::make_unique<rendering::Material>(shader.get(), textureMaterial25.get());
        material37 = std::make_unique<rendering::Material>(shader.get(), textureMaterial37.get());

        // create objects
        createObject(r, 0,
                     TransformComponent{
                         .localPosition = math::Vector3{{0, 1.f, 0}},
                         .localRotation = math::Quaternion::identity,
                         .localScale = math::Vector3::create(3.f)
                     },
                     MeshRendererComponent{
                         .mesh = meshes[0].get(),
                         .material = material25.get()
                     });
        createObject(r, 1, TransformComponent{}, MeshRendererComponent{meshes[1].get(), material25.get()});
        createObject(r, 2, TransformComponent{}, MeshRendererComponent{meshes[2].get(), material37.get()});
        createObject(r, 3, TransformComponent{}, MeshRendererComponent{meshes[3].get(), material37.get()});
        createObject(r, 4, TransformComponent{}, MeshRendererComponent{meshes[4].get(), materialBaseColor.get()});

        // scene
        scene = std::make_unique<scene::Scene>();

        // editor UI
        ui = std::make_unique<editor::UI>(device, window, shaderLibrary.get());
        ui->setRegistry(&r);

        // input handler
        input = std::make_unique<input::Input>();
    }

    void applicationWillTerminate() override
    {
        ui.reset();
    }

    void render(Window* _window) override
    {
        std::unique_ptr<RenderPassDescriptor> renderPassDescriptor = _window->getRenderPassDescriptor();

        ui->update(*renderPassDescriptor); // todo: move into one render function that takes a scene render function as an argument

        //-------------------------------------------------
        // Update camera transform
        //-------------------------------------------------

        // only update if UI didn't capture keyboard input
        if (!ui->getCapturedKeyboard())
        {
            // set camera aspect ratio based on the current size of the window
            Size size = _window->getContentViewSize();
            camera->setAspectRatio(size.width / size.height);

            auto const xDir = static_cast<float>(input->getKey(KeyCode::D) - input->getKey(KeyCode::A));
            auto const yDir = static_cast<float>(input->getKey(KeyCode::E) - input->getKey(KeyCode::Q));
            auto const zDir = static_cast<float>(input->getKey(KeyCode::W) - input->getKey(KeyCode::S));
            offset += math::Vector3{{xDir, yDir, zDir}} * speed;

            auto const deltaHorizontalRotation = static_cast<float>(input->getKey(KeyCode::RightArrow) - input->getKey(KeyCode::LeftArrow)) * rotationSpeed;
            auto const deltaVerticalRotation = static_cast<float>(input->getKey(KeyCode::UpArrow) - input->getKey(KeyCode::DownArrow)) * rotationSpeed;
            horizontalRotation += deltaHorizontalRotation;
            verticalRotation += deltaVerticalRotation;

            math::Quaternion cameraRotation = math::Quaternion::createFromEulerInRadians(
                math::Vector3{{-math::degreesToRadians(verticalRotation), math::degreesToRadians(horizontalRotation), 0}}
            );

            math::Matrix4 cameraTransform = math::createTranslationRotationScaleMatrix(
                offset, cameraRotation, math::Vector3::one);

            camera->setTransform(cameraTransform);
        }

        //-------------------------------------------------
        // Update transforms of all objects (should be refactored into renderer / scene abstraction)
        //-------------------------------------------------

        // updates the transform components based on the hierarchy
        rendering::computeLocalToWorldMatrices(r);

        //-------------------------------------------------
        // Draw objects with MeshRenderers on the screen (should be refactored into renderer / scene abstraction)
        //-------------------------------------------------

        std::unique_ptr<ICommandBuffer> cmd = commandQueue->getCommandBuffer();

        cmd->beginRenderPass(*renderPassDescriptor);
        cmd->setWindingOrder(WindingOrder::Clockwise);
        cmd->setCullMode(CullMode::Back);
        cmd->setTriangleFillMode(TriangleFillMode::Fill);
        cmd->setDepthStencilState(depthStencilState.get());

        for (auto [entityId, meshRenderer, transform, visible]:
            r.view<MeshRendererComponent, TransformComponent, VisibleComponent>(
                entity::IterationPolicy::UseFirstComponent))
        {
            rendering::Mesh* mesh = meshRenderer.mesh;
            rendering::Material* material = meshRenderer.material;

            cmd->setRenderPipelineState(material->getShader()->getRenderPipelineState());

            cmd->setVertexStageBuffer(mesh->getVertexBuffer(), /*offset*/ 0, /*atIndex*/ 0);
            cmd->setVertexStageBuffer(camera->getCameraDataBuffer(), /*offset*/ 0, /*atIndex*/ 1);

            // set small constant data that is different for each object
            math::Matrix4 localToWorldTransform = transform.localToWorldTransform.transpose();
            cmd->setVertexStageBytes(static_cast<void const*>(&localToWorldTransform),
                /*length*/ sizeof(math::Matrix4),
                /*atIndex*/ 2);

            cmd->setFragmentStageTexture(material->getTexture(), 0);

            cmd->drawIndexedPrimitives(PrimitiveType::Triangle,
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

        std::unique_ptr<ITexture> drawable = window->getDrawable();
        cmd->present(drawable.get());
        cmd->commit();
    }

    void setDevice(IDevice* _device)
    {
        device = _device;
    }

    void setWindow(Window* _window)
    {
        window = _window;
    }

private:
    IDevice* device{nullptr};
    Window* window{nullptr};

    std::unique_ptr<input::Input> input;
    std::unique_ptr<scene::Scene> scene;

    std::unique_ptr<editor::UI> ui;

    std::unique_ptr<ICommandQueue> commandQueue;
    std::unique_ptr<IShaderLibrary> shaderLibrary;
    std::unique_ptr<IDepthStencilState> depthStencilState;

    std::unique_ptr<rendering::Camera> camera;

    std::vector<std::unique_ptr<rendering::Mesh>> meshes;

    // textures
    std::unique_ptr<graphics::ITexture> textureMaterial25;
    std::unique_ptr<graphics::ITexture> textureMaterial37;
    std::unique_ptr<graphics::ITexture> textureBaseColor;

    // shaders
    std::unique_ptr<rendering::Shader> shader;

    // materials
    std::unique_ptr<rendering::Material> material25;
    std::unique_ptr<rendering::Material> material37;
    std::unique_ptr<rendering::Material> materialBaseColor;

    // entity
    Registry r;

    float speed = 1.0f;
    float rotationSpeed = 1.0f;

    math::Vector3 offset = math::Vector3::zero;
    float horizontalRotation = 0.f;
    float verticalRotation = 0.0f;
};

int main(int argc, char* argv[])
{
    // create application, should be done first
    Application application{};

    Editor app{};
    application.setDelegate(&app);

    GraphicsBackend backend = GraphicsBackend::Metal;
    std::unique_ptr<IDevice> device = createDevice(backend);

    WindowDescriptor descriptor{
        .x = 600,
        .y = 500,
        .width = 1300,
        .height = 700,
        .flags = WindowFlags_Default,
        .clearColor = math::Vector4{{0.5f, 1.f, 1.f, 1.f}}
    };
    std::unique_ptr<Window> window = device->createWindow(descriptor);
    window->setTitle("Engine somewhat from scratch");
    window->setMinSize({300, 100});
    window->setRenderDelegate(&app);
    window->setInputDelegate(&app); // app listens to input from the window

    app.setDevice(device.get());
    app.setWindow(window.get());

    // run application
    application.run();

    return 0;
}