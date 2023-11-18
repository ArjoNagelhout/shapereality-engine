#ifndef BORED_ENGINE_APPLICATION_H
#define BORED_ENGINE_APPLICATION_H

#include "renderer/renderer.h"
#include "renderer/window.h"

#include <memory>

namespace engine
{
	// on macOS the operating system calls the application instead of the other way around, so we'll adopt this
	// approach for our framework as well.
	class ApplicationDelegate
	{
	public:
		virtual ~ApplicationDelegate();

		virtual void applicationDidFinishLaunching();
	};

	class Application
	{
	public:
		explicit Application();
		~Application();
		void run();

		// delegate
		ApplicationDelegate* getDelegate(); // change to shared pointer?
		void setDelegate(ApplicationDelegate* delegate);

		// renderer
		renderer::Renderer* getRenderer();

	private:
		// implementation
		struct Implementation;
		std::unique_ptr<Implementation> pImplementation;

		ApplicationDelegate* pDelegate{nullptr};

		// renderer
		std::unique_ptr<renderer::Renderer> pRenderer;
	};
}

#endif //BORED_ENGINE_APPLICATION_H