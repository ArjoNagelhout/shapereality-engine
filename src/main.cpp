#include "application.h"

#include "graphics/window.h"
#include "graphics/graphics.h"
#include "graphics/device.h"
#include "graphics/command_queue.h"
#include "graphics/render_pass.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "graphics/render_pipeline_state.h"
#include "graphics/buffer.h"

#include "graphics/input.h"

#include "renderer/mesh.h"
#include "renderer/camera.h"

#include "math/utils.h"
#include "math/vector.h"
#include "math/vector.inl"

#include <iostream>

using namespace graphics;

// high level implementation of what the app should be doing
class App final : public engine::IApplicationDelegate, public IWindowRenderDelegate, public IWindowInputDelegate
{
public:
	explicit App() = default;

	~App() = default;

	void onEvent(InputEvent const& event, Window* window) override
	{
		std::cout << event.toString() << std::endl;
		if (event.type == InputEventType::Keyboard)
		{
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
			"/Users/arjonagelhout/Documents/Experiments/bored_engine/build/shaders/library.metallib");

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

	void applicationDidFinishLaunching() override
	{
		CommandQueueDescriptor commandQueueDescriptor{};
		pCommandQueue = pDevice->createCommandQueue(commandQueueDescriptor);

		pMesh = std::make_unique<renderer::Mesh>(pDevice);
		pCamera = std::make_unique<renderer::Camera>(pDevice);
		createShader();

		std::cout << sizeof(math::mat4) << std::endl;
	}

	void render(Window* window) override
	{
		// set camera aspect ratio based on the current size of the window
		math::Rect rect = window->getRect();
		pCamera->setAspectRatio(rect.width / rect.height);

		t += 0.2f;

		auto const xDir = static_cast<float>(pressed[d] - pressed[a]);
		auto const yDir = static_cast<float>(pressed[e] - pressed[q]);
		auto const zDir = static_cast<float>(pressed[w] - pressed[s]);

		offset += math::vec3{{-xDir, -yDir, zDir}} * speed;
		std::cout << offset << std::endl;

		//float fov = 20.f + 50.f + 50.f * sin(t);
		float x = 0.0f + 1.0f * sin(t*0.1f);
		float z = 2.f + 0.2f * sin(t+1.f);

		math::vec3 pos = math::vec3{{x, 0, -2}};

		pCamera->setWorldPosition(pos + offset);
		//pCamera->setFieldOfView(math::degreesToRadians(fov));

		//std::cout << pos << std::endl;

		std::unique_ptr<IRenderPass> renderPass = window->getRenderPass();
		std::unique_ptr<ICommandBuffer> cmd = pCommandQueue->getCommandBuffer();

		cmd->beginRenderPass(renderPass.get());

		// rendering code here
		cmd->setRenderPipelineState(pRenderPipelineState.get());
		cmd->setDepthStencilState(pDepthStencilState.get());

//		Viewport viewport{
//			.originX = 0.0f,
//			.originY = 0.0f,
//			.width = 500.0f,
//			.height = 500.0f,
//			.zNear = 0.0f,
//			.zFar = 1.0f
//		};
//
//		cmd->setViewport(viewport);

		cmd->setWindingOrder(WindingOrder::Clockwise);
		cmd->setCullMode(CullMode::None);

		// sets a buffer for the vertex stage
		cmd->setBufferForVertexStage(pMesh->getVertexBuffer(), /*offset*/ 0, /*atIndex*/ 0);
		cmd->setBufferForVertexStage(pCamera->getCameraDataBuffer(), /*offset*/ 0, /*atIndex*/ 1);

		cmd->drawIndexedPrimitives(PrimitiveType::Triangle,
			/*indexCount*/ 36,
			/*indexBuffer*/ pMesh->getIndexBuffer(),
			/*indexBufferOffset*/ 0,
			/*instanceCount*/ 3,
			/*baseVertex*/ 0,
			/*baseInstance*/ 0);

		cmd->endRenderPass(renderPass.get());

		std::unique_ptr<ITexture> drawable = window->getDrawable();
		cmd->present(drawable.get());
		cmd->commit();
		drawable.reset();
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
	std::unique_ptr<renderer::Mesh> pMesh;
	std::unique_ptr<renderer::Camera> pCamera;

	// very simple and dumb temporary way to get key input for moving around
	constexpr static int w = 0;
	constexpr static int a = 1;
	constexpr static int s = 2;
	constexpr static int d = 3;
	constexpr static int q = 4;
	constexpr static int e = 5;
	std::array<int, 6> pressed;

	float speed = 0.2f;

	math::vec3 offset = math::vec3::zero;

	float t = 0;
};

int main(int argc, char* argv[])
{
	// create application, should be done first
	engine::Application application{};

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
	window->setTitle("bored engine");
	window->setMinSize(300, 100);
	window->setRenderDelegate(&app);
	window->setInputDelegate(&app); // app listens to input from the window

	app.setDevice(device.get());

	// run application
	application.run();

	return 0;
}