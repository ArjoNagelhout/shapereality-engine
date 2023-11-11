#include "application.h"

int main( int argc, char* argv[] )
{
	engine::Application application{};
	application.setRendererBackend(renderer::RendererBackend::Metal);
	application.run();

	return 0;
}