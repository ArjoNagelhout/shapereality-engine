#include "application.h"

#include <iostream>

namespace engine
{
	Application::Application() = default;

	Application::~Application() = default;

	void Application::run()
	{
		while (true)
		{
			std::cout << "test" << std::endl;
		}
	}
}