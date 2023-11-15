#include "application.h"

#include <iostream>

namespace engine
{
	struct Application::Implementation
	{
		explicit Implementation() = default;

		~Implementation() = default;
	};

	Application::Application()
	{
		pImpl = std::make_unique<Implementation>();
	}

	Application::~Application()
	{
		pImpl.reset();
	}
}