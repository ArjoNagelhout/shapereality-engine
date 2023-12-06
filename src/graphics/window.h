#ifndef BORED_ENGINE_WINDOW_H
#define BORED_ENGINE_WINDOW_H

#include <memory>

#include "../math/rect.h"
#include "../math/vector.h"

namespace graphics
{
	enum WindowFlags_ : uint32_t
	{
		WindowFlags_None = 0,
		WindowFlags_Borderless = 1 << 0,
		WindowFlags_Titled = 1 << 1,
		WindowFlags_Closable = 1 << 2,
		WindowFlags_Miniaturizable = 1 << 3,
		WindowFlags_Resizable = 1 << 4,
		WindowFlags_UnifiedTitleAndToolbar = 1 << 5,

		WindowFlags_Default = WindowFlags_Titled | WindowFlags_Closable | WindowFlags_Miniaturizable | WindowFlags_Resizable
	};

	class IWindow;

	class IWindowDelegate
	{
	public:
		virtual void render(IWindow* window);
	};

	struct WindowDescription
	{
		int x;
		int y;
		int width;
		int height;
		int flags;
		math::vec4 clearColor;
	};

	class IWindow
	{
	public:
		explicit IWindow(WindowDescription description);

		virtual ~IWindow();

		// set the title of this window
		void setTitle(std::string const& title);

		// show this window (on macOS: deminiaturize)
		void show();

		// hide this window (on macOS: minimize)
		void hide();

		// move and scale this window so that it fills the entire screen
		// note: this is not the same as `fullscreen()`
		void maximize();

		// make this window fullscreen
		void fullscreen();

		//
		void setPosition(int x, int y);

		// set the size of this window
		void setSize(int width, int height);

		// set the minimum horizontal and vertical dimensions of this window when resizing
		void setMinSize(int width, int height);

		// set the maximum horizontal and vertical dimensions of this window when resizing
		void setMaxSize(int width, int height);

		// get the position and size of this window
		[[nodiscard]] math::Rect getRect() const;

		//
		void setRect(math::Rect const& rect); // set both position and size

		// get the delegate of this window
		[[nodiscard]] IWindowDelegate* getDelegate() const;

		// set the delegate of this window
		void setDelegate(IWindowDelegate* delegate);

	protected:
		// platform specific implementation that can be accessed by a graphics backend subclass of IWindow
		// to automatically create a surface / context that can be drawn to.
		class Implementation;
		std::unique_ptr<Implementation> pImplementation;

		// the delegate is responsible for rendering this window's contents
		IWindowDelegate* pDelegate{nullptr};
	};
}

#endif //BORED_ENGINE_WINDOW_H
