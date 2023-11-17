#ifndef BORED_ENGINE_WINDOW_H
#define BORED_ENGINE_WINDOW_H

#include <memory>

#include "view.h"

#include "math/rect.h"

// todo: fix window opening in tab instead of as separate window if this was specified in settings by user.
// todo: add support for NSPanel on macOS

namespace engine
{
	enum WindowFlags_ : uint32_t
	{
		WindowFlags_None					= 0,
		WindowFlags_Borderless				= 1 << 0,
		WindowFlags_Titled					= 1 << 1,
		WindowFlags_Closable				= 1 << 2,
		WindowFlags_Miniaturizable			= 1 << 3,
		WindowFlags_Resizable				= 1 << 4,
		WindowFlags_UnifiedTitleAndToolbar	= 1 << 5
	};

	class Window
	{
	public:
		explicit Window(int const& x, int const& y, int const& width, int const& height, int const& flags = WindowFlags_Titled | WindowFlags_Closable | WindowFlags_Miniaturizable | WindowFlags_Resizable);
		~Window();
		void setTitle(std::string const& title);
		void show();
		void hide();
		void minimize();
		void maximize();
		void fullscreen();
		void setPosition(int const& x, int const& y);
		void setSize(int const& width, int const& height);
		void setMinSize(int const& width, int const& height);
		void setMaxSize(int const& width, int const& height);
		Rect getRect();
		void setRect(Rect const& rect); // set both position and size
		View* getContentView();
		void setContentView(View* view); // sets the contents of the view

	private:
		View* pContentView{nullptr};

		struct Implementation;

		std::unique_ptr<Implementation> pImpl;
	};
}

#endif //BORED_ENGINE_WINDOW_H
