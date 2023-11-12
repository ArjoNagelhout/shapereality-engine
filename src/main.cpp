#include "application.h"

#include "window.h"

#include <iostream>

#include "window.h"

int main( int argc, char* argv[] )
{
	engine::Window newWindow{22, 33, 500, 50, engine::WindowFlags_None};
	newWindow.setTitle("Test guys, garbage here");

	engine::Window newWindow2{22, 33, 300, 300, engine::WindowFlags_Titled | engine::WindowFlags_Closable};
	newWindow2.setTitle("Test guys, garbage here");

	engine::Window newWindow3{22, 33, 200, 200, engine::WindowFlags_Titled | engine::WindowFlags_Closable | engine::WindowFlags_Miniaturizable | engine::WindowFlags_Resizable};
	newWindow3.setTitle("Eveythang");

	engine::Window unifiedWindow{22, 33, 200, 200, engine::WindowFlags_UnifiedTitleAndToolbar | engine::WindowFlags_Titled | engine::WindowFlags_Resizable};
	unifiedWindow.setTitle("unified");

	engine::Application application{};
	application.setRendererBackend(renderer::RendererBackend::Metal);
	application.run();

	return 0;
}