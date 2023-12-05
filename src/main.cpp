#include "application.h"
#include "renderer/window.h"
#include "renderer/texture.h"
#include "scene/scene.h"
#include "renderer/command_buffer.h"

#include <iostream>

#include "math/matrix.h"
#include "math/matrix.inl"
#include "math/vector.h"
#include "math/vector.inl" // inline definitions
#include "math/ray.h"
#include "math/ray.inl"

// high level implementation of what the app should be doing
class App final : public engine::IApplicationDelegate, public renderer::IWindowDelegate
{
public:
	explicit App()
	{
		pScene = std::make_unique<scene::Scene>();
		pTexture = renderer::create<renderer::Texture>(1024, 1024, renderer::TextureFormat::RGBA8Unorm_sRGB);
	}

	~App()
	{
		pScene.reset();
	}

	void applicationDidFinishLaunching() override
	{

	}

	void render(renderer::Window* window) override
	{
		pScene->render();
	}

private:
	std::unique_ptr<scene::Scene> pScene;
	std::unique_ptr<renderer::Texture> pTexture;
};

int main( int argc, char* argv[] )
{
	// create application, should be done first
	engine::Application application{};

	App app{};
	application.setDelegate(&app);

	// set renderer backend
	renderer::Graphics* renderer = application.getGraphics();
	renderer->setGraphicsBackendType(renderer::GraphicsBackendType::Metal);

	// add window
	std::unique_ptr<renderer::Window> newWindow = renderer::create<renderer::Window>(500, 500, 500, 400);
	newWindow->setTitle("bored engine");
	newWindow->setMinSize(300, 100);
	newWindow->setSize(900, 700);
	newWindow->setDelegate(&app);

	// run application
	application.run();

	return 0;
}