#include "application.h"

#include "graphics/device.h"

// high level implementation of what the app should be doing
class App final : public engine::IApplicationDelegate, public graphics::IWindowDelegate
{
public:
	explicit App() = default;

	~App() = default;

	void applicationDidFinishLaunching() override
	{

	}

	void render(graphics::IWindow* window) override
	{

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

//	// add window
//	std::unique_ptr<graphics::IWindow> newWindow = graphics::createWindow(backend);


	// run application
	application.run();

	return 0;
}