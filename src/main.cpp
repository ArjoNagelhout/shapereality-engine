#include "application.h"
#include "graphics/window.h"
#include "graphics/texture.h"
#include "scene/scene.h"
#include "graphics/command_buffer.h"

#include <iostream>

#include "math/matrix.h"
#include "math/matrix.inl"
#include "math/vector.h"
#include "math/vector.inl" // inline definitions
#include "math/ray.h"
#include "math/ray.inl"

// high level implementation of what the app should be doing
class App final : public engine::IApplicationDelegate, public graphics::IWindowDelegate
{
public:
	explicit App()
	{
		pScene = std::make_unique<scene::Scene>();
		pTexture = graphics::create<graphics::Texture>(1024, 1024, graphics::TextureFormat::RGBA8Unorm_sRGB);
	}

	~App()
	{
		pScene.reset();
	}

	void applicationDidFinishLaunching() override
	{

	}

	void render(graphics::Window* window) override
	{
		pScene->render();
	}

private:
	std::unique_ptr<scene::Scene> pScene;
	std::unique_ptr<graphics::Texture> pTexture;
};

int main( int argc, char* argv[] )
{
	// create application, should be done first
	engine::Application application{};

	App app{};
	application.setDelegate(&app);

	// set renderer backend
	graphics::Graphics* renderer = application.getGraphics();
	renderer->setGraphicsBackendType(graphics::GraphicsBackendType::Metal);

	// add window
	std::unique_ptr<graphics::Window> newWindow = graphics::create<graphics::Window>(500, 500, 500, 400);
	newWindow->setTitle("bored engine");
	newWindow->setMinSize(300, 100);
	newWindow->setSize(900, 700);
	newWindow->setDelegate(&app);

	// run application
	application.run();

	return 0;
}