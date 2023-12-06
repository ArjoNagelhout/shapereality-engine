#ifndef BORED_ENGINE_WINDOW_H
#define BORED_ENGINE_WINDOW_H

#include <memory>

#include "../math/rect.h"

namespace graphics
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

	class IWindow;

	class IWindowDelegate
	{
	public:
		virtual void render(IWindow* window);
	};

	class IWindow
	{
	public:
		explicit IWindow(int const& x, int const& y, int const& width, int const& height, int const& flags = WindowFlags_Titled | WindowFlags_Closable | WindowFlags_Miniaturizable | WindowFlags_Resizable);
		virtual ~IWindow();

		void setTitle(std::string const& title);
		void show();
		void hide();
		void maximize();
		void fullscreen();
		void setPosition(int const& x, int const& y);
		void setSize(int const& width, int const& height);
		void setMinSize(int const& width, int const& height);
		void setMaxSize(int const& width, int const& height);
		math::Rect getRect();
		void setRect(math::Rect const& rect); // set both position and size

		[[nodiscard]] IWindowDelegate* getDelegate() const;
		void setDelegate(IWindowDelegate* delegate);

	private:
		class Implementation;
		std::unique_ptr<Implementation> pImplementation;

		IWindowDelegate* pDelegate{nullptr};
	};
}

#endif //BORED_ENGINE_WINDOW_H
