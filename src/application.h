#ifndef BORED_ENGINE_APPLICATION_H
#define BORED_ENGINE_APPLICATION_H

#include "renderer/renderer.h"
#include "window.h"

#include <memory>

namespace engine
{
	class Application
	{
	public:
		explicit Application();

		~Application();

		void run();

		// windows
		std::vector<std::unique_ptr<Window>>& getWindows();

		// rendering
		[[nodiscard]] renderer::RendererBackend getRendererBackend() const;

		void setRendererBackend(renderer::RendererBackend const& rendererBackend);

	private:
		struct Implementation;

		std::unique_ptr<Implementation> pImpl;

		std::vector<std::unique_ptr<Window>> pWindows;

		renderer::RendererBackend rendererBackend{renderer::RendererBackend::None};
		std::unique_ptr<renderer::Renderer> pRenderer;


	};
}

#endif //BORED_ENGINE_APPLICATION_H