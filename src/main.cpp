#include "application.h"

#include "window.h"

int main( int argc, char* argv[] )
{
	engine::Window window{10, 10, 512, 512};

	engine::Application application{};
	application.setRendererBackend(renderer::RendererBackend::Metal);
	application.run();

	return 0;
}