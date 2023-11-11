#include "application.h"

#include "window.h"

int main( int argc, char* argv[] )
{
	engine::Window window{10, 10, 512, 512};

	window.setTitle("soepie");

	engine::Window window2{10, 10, 300, 300};
	window2.setTitle("something");

	engine::Application application{};
	application.setRendererBackend(renderer::RendererBackend::Metal);
	application.run();

	return 0;
}