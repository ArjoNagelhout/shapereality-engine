#include "graphics/application.h"

#include "math/vector.h"
#include "math/vector.inl"
#include "math/matrix.h"
#include "math/matrix.inl"

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

#include "renderer/mesh.h"
#include "renderer/camera.h"

#include "assets/import/mesh_importer.h"
#include "assets/import/texture_importer.h"

#include <iostream>

using namespace graphics;

// high level implementation of what the app should be doing
class App final : public IApplicationDelegate, public IWindowRenderDelegate, public IWindowInputDelegate
{
public:
	explicit App() = default;

	~App() = default;

	void onEvent(InputEvent const& event, Window* window) override
	{
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
				case KeyCode::W:
					index = w;
					break;
				case KeyCode::A:
					index = a;
					break;
				case KeyCode::S:
					index = s;
					break;
				case KeyCode::D:
					index = d;
					break;
				case KeyCode::Q:
					index = q;
					break;
				case KeyCode::E:
					index = e;
					break;
				default:
					return;
			}

			pressed[index] = value;
		}
	}

	void createShader()
	{
		// shader library contains compiled shader source code
		//
		// generated using
		// python compile_shaders.py ../data/shaders ../build/shaders ../build/shaders/library
		//
		pShaderLibrary = pDevice->createShaderLibrary(
			"/Users/arjonagelhout/Documents/ShapeReality/shapereality/build/shaders/library.metallib");

		ShaderFunctionDescriptor vertexDescriptor{
			.entryPoint = "simple_vertex",
			.type = ShaderFunctionType::Vertex
		};
		std::unique_ptr<IShaderFunction> pVertexFunction = pShaderLibrary->createShaderFunction(vertexDescriptor);

		ShaderFunctionDescriptor fragmentDescriptor{
			.entryPoint = "simple_fragment",
			.type = ShaderFunctionType::Fragment
		};
		std::unique_ptr<IShaderFunction> pFragmentFunction = pShaderLibrary->createShaderFunction(fragmentDescriptor);

		RenderPipelineDescriptor renderPipelineDescriptor{
			.vertexFunction = pVertexFunction.get(),
			.fragmentFunction = pFragmentFunction.get(),
			.colorAttachments = {
				{.pixelFormat = PixelFormat::BGRA8Unorm_sRGB}
			},
			.depthAttachmentPixelFormat = PixelFormat::Depth16Unorm,
		};

		pRenderPipelineState = pDevice->createRenderPipelineState(renderPipelineDescriptor);

		DepthStencilDescriptor depthStencilDescriptor{
			.depthCompareFunction = CompareFunction::Less,
			.depthWriteEnabled = true,
		};

		pDepthStencilState = pDevice->createDepthStencilState(depthStencilDescriptor);
	};

	// todo: these assets should only converted once, and then it should simply use a cached version.
	// 		 so a project has a corresponding cache that contains the converted assets.
	//
	// this importing should eventually be abstracted away so that we can specify which assets to load.
	// on runtime, it should only read the binary formats that don't have to imported.
	void importMeshes()
	{
		// import meshes
		std::filesystem::path meshPath = "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/scene.gltf";
		assets::MeshImportDescriptor meshImportDescriptor{

		};

		assets::MeshImportResult importMeshResult = assets::importMesh(pDevice, meshPath, meshImportDescriptor, pMeshes);
		if (!importMeshResult.success)
		{
			exit(1);
		}
	}

	void importTexture()
	{
		// import textures
		//std::filesystem::path texturePath = "/Users/arjonagelhout/Documents/ShapeReality/shapereality/data/models/sea_house/textures/11112_sheet_Material__25_baseColor.png";
		std::filesystem::path texturePath = "/Users/arjonagelhout/Desktop/cute.png";
		assets::TextureImportDescriptor textureImportDescriptor{

		};
		assets::TextureImportResult importTextureResult = assets::importTexture(pDevice, texturePath, textureImportDescriptor, pTexture);
		if (!importTextureResult.success)
		{
			std::cout << importTextureResult.errorMessage << std::endl;
			exit(1);
		}
	}

	void applicationDidFinishLaunching() override
	{
		CommandQueueDescriptor commandQueueDescriptor{};
		pCommandQueue = pDevice->createCommandQueue(commandQueueDescriptor);
		pCamera = std::make_unique<renderer::Camera>(pDevice);
		createShader();
		importMeshes();
		importTexture();
	}

	void render(Window* window) override
	{
		// set camera aspect ratio based on the current size of the window
		math::Rect rect = window->getRect();
		pCamera->setAspectRatio(rect.width / rect.height);

		// very crude input system
		t += 0.5f;
		float x = 0.1f * sin(t * 0.1f);
		math::vec3 pos = math::vec3{{x, 0, 0}};

		auto const xDir = static_cast<float>(pressed[d] - pressed[a]);
		auto const yDir = static_cast<float>(pressed[e] - pressed[q]);
		auto const zDir = static_cast<float>(pressed[w] - pressed[s]);
		offset += math::vec3{{xDir, yDir, zDir}} * speed;

		pCamera->setWorldPosition(pos + offset);

		std::unique_ptr<IRenderPass> renderPass = window->getRenderPass();
		std::unique_ptr<ICommandBuffer> cmd = pCommandQueue->getCommandBuffer();

		cmd->beginRenderPass(renderPass.get());

		// rendering code here
		cmd->setRenderPipelineState(pRenderPipelineState.get());
		cmd->setDepthStencilState(pDepthStencilState.get());
		cmd->setWindingOrder(WindingOrder::Clockwise);
		cmd->setCullMode(CullMode::None);

		for (auto& mesh : pMeshes)
		{
			// sets a buffer for the vertex stage
			cmd->setVertexStageBuffer(mesh->getVertexBuffer(), /*offset*/ 0, /*atIndex*/ 0);
			cmd->setVertexStageBuffer(pCamera->getCameraDataBuffer(), /*offset*/ 0, /*atIndex*/ 1);

			cmd->setFragmentStageTexture(pTexture.get(), 0);

			cmd->drawIndexedPrimitives(PrimitiveType::Triangle,
				/*indexCount*/ mesh->getIndexCount(),
				/*indexBuffer*/ mesh->getIndexBuffer(),
				/*indexBufferOffset*/ 0,
				/*instanceCount*/ 1,
				/*baseVertex*/ 0,
				/*baseInstance*/ 0);
		}

		cmd->endRenderPass(renderPass.get());

		std::unique_ptr<ITexture> drawable = window->getDrawable();
		cmd->present(drawable.get());
		cmd->commit();
	}

	// todo: move
	void setDevice(IDevice* device)
	{
		pDevice = device;
	}

private:
	IDevice* pDevice{nullptr};
	Window* pWindow{nullptr};
	std::unique_ptr<ICommandQueue> pCommandQueue;
	std::unique_ptr<IShaderLibrary> pShaderLibrary;
	std::unique_ptr<IRenderPipelineState> pRenderPipelineState;
	std::unique_ptr<IDepthStencilState> pDepthStencilState;
	std::vector<std::unique_ptr<renderer::Mesh>> pMeshes;
	std::unique_ptr<graphics::ITexture> pTexture;
	std::unique_ptr<renderer::Camera> pCamera;

	// very simple and dumb temporary way to get key input for moving around
	constexpr static int w = 0;
	constexpr static int a = 1;
	constexpr static int s = 2;
	constexpr static int d = 3;
	constexpr static int q = 4;
	constexpr static int e = 5;
	std::array<int, 6> pressed{};

	float speed = 1.0f;

	math::vec3 offset = math::vec3::zero;

	float t = 0;
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
		.x = 500,
		.y = 500,
		.width = 500,
		.height = 500,
		.flags = WindowFlags_Default,
		.clearColor = math::vec4{{0.5f, 1.f, 1.f, 1.f}}
	};
	std::unique_ptr<Window> window = device->createWindow(descriptor);
	window->setTitle("ShapeReality");
	window->setMinSize(300, 100);
	window->setRenderDelegate(&app);
	window->setInputDelegate(&app); // app listens to input from the window

	app.setDevice(device.get());

	// run application
	application.run();

	return 0;
}