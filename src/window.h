#ifndef BORED_ENGINE_WINDOW_H
#define BORED_ENGINE_WINDOW_H

#include <string>

#if defined(__APPLE__)

#include <AppKit/AppKit.hpp>

#elif defined(WIN32)



#endif

namespace engine
{
	class Window
	{
	public:
		explicit Window(int x, int y, int width, int height);

		~Window();

		void SetTitle(std::string const& title);

	private:

#if defined(__APPLE__)

		NS::Window* pWindow;

#elif defined(WIN32)

#endif

	};
}

#endif //BORED_ENGINE_WINDOW_H
