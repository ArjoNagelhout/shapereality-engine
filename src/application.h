#ifndef BORED_ENGINE_APPLICATION_H
#define BORED_ENGINE_APPLICATION_H

#include "renderer/renderer.h"
#include "renderer/window.h"

#include <memory>

namespace engine
{
	// on macOS the operating system calls the application instead of the other way around, so we'll adopt this
	// approach for our framework as well.
	class IApplicationDelegate
	{
	public:
		virtual void applicationDidFinishLaunching();
	};

	class Application
	{
	public:
		explicit Application();
		~Application();
		void run();

		// delegate
		IApplicationDelegate* getDelegate(); // change to shared pointer?
		void setDelegate(IApplicationDelegate* delegate);

		// renderer
		renderer::Renderer* getRenderer();

	private:
		// implementation
		struct Implementation;
		std::unique_ptr<Implementation> pImplementation;

		IApplicationDelegate* pDelegate{nullptr};

		// renderer
		std::unique_ptr<renderer::Renderer> pRenderer;
	};
}

#endif //BORED_ENGINE_APPLICATION_H