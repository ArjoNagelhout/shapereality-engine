#include "application.h"

#include "graphics/device.h"

#include <iostream>

// high level implementation of what the app should be doing
class App final : public engine::IApplicationDelegate, public graphics::IWindowDelegate
{
public:
	explicit App() = default;

	~App() = default;

	void applicationDidFinishLaunching() override
	{
		//graphics::ICommandQueue
	}

	void render(graphics::IWindow* window) override
	{
		std::cout << "rendererer" << std::endl;

		// we need a command queue. these are used throughout the lifetime of the app
	}
};

int main(int argc, char* argv[] )
{
	// create application, should be done first
	engine::Application application{};

	App app{};
	application.setDelegate(&app);

	graphics::GraphicsBackend backend = graphics::GraphicsBackend::Metal;

	std::unique_ptr<graphics::IDevice> device = graphics::createDevice(backend);

	graphics::WindowDescription desc{
		.x = 500,
		.y = 500,
		.width = 500,
		.height = 500,
		.flags = graphics::WindowFlags_Default,
		.clearColor = math::vec4{{0.5f, 1.f, 1.f, 1.f}}
	};
	std::unique_ptr<graphics::IWindow> window = device->createWindow(desc);
	window->setTitle("bored engine");
	window->setMinSize(300, 100);
	window->setDelegate(&app);

	// run application
	application.run();

	return 0;
}