#include "application.h"
#include "window.h"

#include <iostream>

class ApplicationDelegate : public engine::ApplicationDelegate
{
public:
	~ApplicationDelegate() override = default;

	void applicationDidFinishLaunching() override
	{
		std::cout << "we finished launching, happy" << std::endl;
	}
};

class RendererDelegate : public renderer::RendererDelegate
{
public:
	~RendererDelegate() override = default;

	void render(engine::Window *window) override
	{
		std::cout << "fuck we need to render the thing" << std::endl;
	}
};

int main( int argc, char* argv[] )
{
	ApplicationDelegate applicationDelegate{};
	RendererDelegate rendererDelegate{};

	engine::Window newWindow(22, 33, 500, 400);
	newWindow.setTitle("Test guys, garbage here");

	engine::Application application{};
	application.setDelegate(&applicationDelegate);

	application.setRendererBackend(renderer::RendererBackend::Metal);
	renderer::Renderer* renderer = application.getRenderer();
	renderer->setDelegate(&rendererDelegate);
	renderer->addWindow(&newWindow);

	application.run();

	return 0;
}