#ifndef BORED_ENGINE_APPLICATION_H
#define BORED_ENGINE_APPLICATION_H

#include <memory>

namespace graphics
{
	// on macOS the operating system calls the application instead of the other way around, so we'll adopt this
	// approach for our framework as well.
	class IApplicationDelegate
	{
	public:
		virtual void applicationDidFinishLaunching();
	};

	class Application final
	{
	public:
		explicit Application();
		~Application();

		void run();

		[[nodiscard]] IApplicationDelegate* getDelegate() const;

		void setDelegate(IApplicationDelegate* delegate);

		struct Implementation;

	private:
		std::unique_ptr<Implementation> pImplementation;

		IApplicationDelegate* pDelegate{nullptr};
	};
}

#endif //BORED_ENGINE_APPLICATION_H