#ifndef BORED_ENGINE_WINDOW_H
#define BORED_ENGINE_WINDOW_H

#include <memory>

#include "graphics.h"
#include "../math/rect.h"

namespace renderer
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

	class Window;

	class WindowRendererImplementation
	{
	public:
		explicit WindowRendererImplementation(Window* window);
		virtual ~WindowRendererImplementation();

	protected:
		Window* pWindow;
	};

	class WindowPlatformImplementation;

	class IWindowDelegate
	{
	public:
		virtual void render(Window* window);
	};

	class Window final : public GraphicsObject
	{
	public:
		explicit Window(int const& x, int const& y, int const& width, int const& height, int const& flags = WindowFlags_Titled | WindowFlags_Closable | WindowFlags_Miniaturizable | WindowFlags_Resizable);
		~Window() override;

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

		IWindowDelegate* getDelegate();
		void setDelegate(IWindowDelegate* delegate);

		void onGraphicsBackendChanged(GraphicsBackendType const& rendererBackendType) override;

		WindowPlatformImplementation* getPlatformImplementation();
		WindowRendererImplementation* getRendererImplementation();

	private:
		std::unique_ptr<WindowPlatformImplementation> pPlatformImplementation;
		std::unique_ptr<WindowRendererImplementation> pRendererImplementation;

		IWindowDelegate* pDelegate{nullptr};
	};
}

#endif //BORED_ENGINE_WINDOW_H
