#include "application.h"

#include "graphics/device.h"
#include "graphics/command_queue.h"
#include "graphics/render_pass.h"

#include <iostream>

// high level implementation of what the app should be doing
class App final : public engine::IApplicationDelegate, public graphics::IWindowDelegate
{
public:
	explicit App() = default;

	~App() = default;

	void applicationDidFinishLaunching() override
	{
		graphics::CommandQueueDescriptor commandQueueDescription{};
		pCommandQueue = pDevice->createCommandQueue(commandQueueDescription);

		graphics::RenderPassDescriptor renderPassDescription{};
		pRenderPass = pDevice->createRenderPass(renderPassDescription);

		pWindowRenderPass = pWindow->getRenderPass();
	}

	void render(graphics::Window* window) override
	{
		if (window != pWindow)
		{
			return;
		}

		// get command buffer from pCommandQueue
		std::unique_ptr<graphics::ICommandBuffer> cmd = pCommandQueue->getCommandBuffer();

		std::cout << "rendererer" << std::endl;

		cmd->beginRenderPass(pRenderPass.get());

		cmd->endRenderPass(pRenderPass.get());
	}

	// todo: move
	void setDevice(graphics::IDevice* device)
	{
		pDevice = device;
	}

	// todo: move
	void setWindow(graphics::Window* window)
	{
		pWindow = window;
	}

private:
	graphics::IDevice* pDevice{nullptr};
	graphics::Window* pWindow{nullptr};
	std::unique_ptr<graphics::ICommandQueue> pCommandQueue;
	std::unique_ptr<graphics::IRenderPass> pRenderPass;
	std::unique_ptr<graphics::IRenderPass> pWindowRenderPass;
};

int main(int argc, char* argv[] )
{
	// create application, should be done first
	engine::Application application{};

	App app{};
	application.setDelegate(&app);

	graphics::GraphicsBackend backend = graphics::GraphicsBackend::Metal;
	std::unique_ptr<graphics::IDevice> device = graphics::createDevice(backend);

	graphics::WindowDescriptor desc{
		.x = 500,
		.y = 500,
		.width = 500,
		.height = 500,
		.flags = graphics::WindowFlags_Default,
		.clearColor = math::vec4{{0.5f, 1.f, 1.f, 1.f}}
	};
	std::unique_ptr<graphics::Window> window = device->createWindow(desc);
	window->setTitle("bored engine");
	window->setMinSize(300, 100);
	window->setDelegate(&app);

	app.setDevice(device.get());
	app.setWindow(window.get());

	// run application
	application.run();

	return 0;
}