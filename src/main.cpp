#include "application.h"

#include "window.h"

#include <iostream>

#include "window.h"

int main( int argc, char* argv[] )
{
	engine::Window newWindow(22, 33, 500, 400);
	newWindow.setTitle("Test guys, garbage here");

	engine::Application application{};
	application.setRendererBackend(renderer::RendererBackend::Metal);
	renderer::Renderer* renderer = application.getRenderer();
	renderer->addWindow(&newWindow);

	application.run();

	return 0;
}