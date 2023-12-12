#include "application.h"

#include "graphics/device.h"
#include "graphics/command_queue.h"
#include "graphics/render_pass.h"
#include "graphics/texture.h"

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
	}

	void render(graphics::Window* window) override
	{
		std::unique_ptr<graphics::IRenderPass> renderPass = window->getRenderPass();
		std::unique_ptr<graphics::ICommandBuffer> cmd = pCommandQueue->getCommandBuffer();

		cmd->beginRenderPass(renderPass.get());

		// rendering code here

		// we need to get a render pipeline state object


		cmd->endRenderPass(renderPass.get());

		std::unique_ptr<graphics::ITexture> drawable = window->getDrawable();
		cmd->present(drawable.get());
		cmd->commit();
	}

	// todo: move
	void setDevice(graphics::IDevice* device)
	{
		pDevice = device;
	}

private:
	graphics::IDevice* pDevice{nullptr};
	graphics::Window* pWindow{nullptr};
	std::unique_ptr<graphics::ICommandQueue> pCommandQueue;
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

	// run application
	application.run();

	return 0;
}