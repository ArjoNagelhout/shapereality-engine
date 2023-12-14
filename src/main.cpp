#include "application.h"

#include "graphics/device.h"
#include "graphics/command_queue.h"
#include "graphics/render_pass.h"
#include "graphics/texture.h"
#include "graphics/shader.h"
#include "graphics/render_pipeline_state.h"
#include "graphics/buffer.h"

#include <iostream>

using namespace graphics;

// high level implementation of what the app should be doing
class App final : public engine::IApplicationDelegate, public IWindowDelegate
{
public:
	explicit App() = default;

	~App() = default;

	void createBuffers()
	{
		std::array<int, 3> indices{{0, 1, 2}};

		BufferDescriptor indexBufferDescriptor{
			.type = BufferDescriptor::Type::Index,
			.data = &indices,
			.length = indices.size(),
			.stride = sizeof(int)
		};
		pIndexBuffer = pDevice->createBuffer(indexBufferDescriptor);

		std::array<math::vec3, 3> vertices{
			math::vec3{{0, 0, 0}},
			math::vec3{{1, 0, 0}},
			math::vec3{{1, 1, 0}},
		};

		BufferDescriptor vertexBufferDescriptor{
			.type = BufferDescriptor::Type::Vertex,
			.data = &vertices,
			.length = sizeof(vertices),
			.stride = sizeof(math::vec3)
		};
		pVertexBuffer = pDevice->createBuffer(vertexBufferDescriptor);
	}

	void createShader()
	{
		// shader library contains compiled shader source code
		//
		// generated using
		// python compile_shaders.py ../data/shaders ../build/shaders ../build/shaders/library
		//
		pShaderLibrary = pDevice->createShaderLibrary("/Users/arjonagelhout/Documents/Experiments/bored_engine/build/shaders/library.metallib");

		ShaderFunctionDescriptor vertexDescriptor{
			.entryPoint = "main_vertex",
			.type = ShaderFunctionType::Vertex
		};
		std::unique_ptr<IShaderFunction> pVertexFunction = pShaderLibrary->createShaderFunction(vertexDescriptor);

		ShaderFunctionDescriptor fragmentDescriptor{
			.entryPoint = "main_fragment",
			.type = ShaderFunctionType::Fragment
		};
		std::unique_ptr<IShaderFunction> pFragmentFunction = pShaderLibrary->createShaderFunction(fragmentDescriptor);

		RenderPipelineDescriptor renderPipelineDescriptor{
			.vertexFunction = pVertexFunction.get(),
			.fragmentFunction = pFragmentFunction.get()
		};

		pRenderPipelineState = pDevice->createRenderPipelineState(renderPipelineDescriptor);
	};

	void applicationDidFinishLaunching() override
	{
		CommandQueueDescriptor commandQueueDescriptor{};
		pCommandQueue = pDevice->createCommandQueue(commandQueueDescriptor);

		createShader();
		createBuffers();
	}

	void render(Window* window) override
	{
		std::unique_ptr<IRenderPass> renderPass = window->getRenderPass();
		std::unique_ptr<ICommandBuffer> cmd = pCommandQueue->getCommandBuffer();

		cmd->beginRenderPass(renderPass.get());

		// rendering code here
		cmd->setRenderPipelineState(pRenderPipelineState.get());
		cmd->setWindingOrder(WindingOrder::Clockwise);

		cmd->setBuffer(pVertexBuffer.get(), /*offset: */0, /*atIndex: */0);
		//cmd->drawIndexedPrimitives()

		// we need to get a render pipeline state object

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
	std::unique_ptr<IBuffer> pVertexBuffer;
	std::unique_ptr<IBuffer> pIndexBuffer;
};

int main(int argc, char* argv[] )
{
	// create application, should be done first
	engine::Application application{};

	App app{};
	application.setDelegate(&app);

	GraphicsBackend backend = GraphicsBackend::Metal;
	std::unique_ptr<IDevice> device = createDevice(backend);

	WindowDescriptor desc{
		.x = 500,
		.y = 500,
		.width = 500,
		.height = 500,
		.flags = WindowFlags_Default,
		.clearColor = math::vec4{{0.5f, 1.f, 1.f, 1.f}}
	};
	std::unique_ptr<Window> window = device->createWindow(desc);
	window->setTitle("bored engine");
	window->setMinSize(300, 100);
	window->setDelegate(&app);

	app.setDevice(device.get());

	// run application
	application.run();

	return 0;
}