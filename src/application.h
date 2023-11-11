#ifndef BORED_ENGINE_APPLICATION_H
#define BORED_ENGINE_APPLICATION_H

#include "renderer/renderer.h"

#include <memory>

namespace engine
{
	class Application
	{
	public:
		explicit Application();

		~Application();

		void run();

	private:
		std::unique_ptr<renderer::Renderer> renderer;
	};
}

#endif //BORED_ENGINE_APPLICATION_H