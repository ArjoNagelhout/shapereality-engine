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

		[[nodiscard]] renderer::RendererBackend getRendererBackend() const;

		void setRendererBackend(renderer::RendererBackend const& rendererBackend);

	private:
		renderer::RendererBackend rendererBackend{renderer::RendererBackend::None};
		std::unique_ptr<renderer::Renderer> renderer;
	};
}

#endif //BORED_ENGINE_APPLICATION_H