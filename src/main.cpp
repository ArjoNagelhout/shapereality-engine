#include "application.h"

#include "window.h"

#include "window_test.h"

#include <iostream>

#include "window_new.h"

int main( int argc, char* argv[] )
{
	WindowNew newWindow{22, 33, 500, 50};

	int x{0};
	int y{0};

	newWindow.setTitle("Hey guys, garbage here");

	std::cout << x << ", " << y << " wee " << std::endl;

//	WindowTest test{};

//	engine::Window window{10, 10, 512, 512};
//	window.setTitle("soepie");
//
//	engine::Window window2{10, 10, 300, 300};
//	window2.setTitle("something");
//
	engine::Application application{};
	application.setRendererBackend(renderer::RendererBackend::Metal);
	application.run();

	return 0;
}