#include "application.h"

#include <iostream>

namespace engine
{
	struct Application::Implementation
	{
		explicit Implementation()
		{
			std::cout << "push de pimpl" << std::endl;
		}

		~Implementation()
		{
			std::cout << "en pop de pimpl" << std::endl;
		}
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