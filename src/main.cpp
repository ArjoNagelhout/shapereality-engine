#include "application.h"
#include "graphics/window.h"

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

int main( int argc, char* argv[] )
{
	// create application, should be done first
	engine::Application application{};

	App app{};
	application.setDelegate(&app);

	graphics::GraphicsBackend backend = graphics::GraphicsBackend::Metal;

	std::unique_ptr<graphics::IDevice> device = graphics::createDevice(backend);

//	// add window
//	std::unique_ptr<graphics::IWindow> newWindow = graphics::createWindow(backend);
//	newWindow->setTitle("bored engine");
//	newWindow->setMinSize(300, 100);
//	newWindow->setSize(900, 700);
//	newWindow->setDelegate(&app);

	// run application
	application.run();

	return 0;
}