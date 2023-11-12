#include "application.h"

#include "window.h"

#include <iostream>

#include "window.h"

int main( int argc, char* argv[] )
{
	engine::Window newWindow{22, 33, 500, 50};
	newWindow.setTitle("Test guys, garbage here");

	engine::Window newWindow2{22, 33, 300, 300};
	newWindow2.setTitle("Test guys, garbage here");

	engine::Window newWindow3{22, 33, 200, 200};
	newWindow3.setTitle("Eveythang");

	engine::Window unifiedWindow{22, 33, 200, 200};
	unifiedWindow.setTitle("unified");

	engine::Application application{};
	application.setRendererBackend(renderer::RendererBackend::Metal);
	application.run();

	return 0;
}