#include "application.h"

#include "window.h"

#include <iostream>

#include "window.h"

class TestDelegate : public renderer::Delegate
{
public:
	~TestDelegate() override = default;

	void render(engine::Window *window) override
	{
		std::cout << "fuck we need to render the thing" << std::endl;
	}
};

int main( int argc, char* argv[] )
{
	TestDelegate delegate{};

	engine::Window newWindow(22, 33, 500, 400);
	newWindow.setTitle("Test guys, garbage here");

	engine::Application application{};
	application.setRendererBackend(renderer::RendererBackend::Metal);
	renderer::Renderer* renderer = application.getRenderer();
	renderer->setDelegate(&delegate);
	renderer->addWindow(&newWindow);

	application.run();

	return 0;
}