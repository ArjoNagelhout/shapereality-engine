#ifndef BORED_ENGINE_WINDOW_H
#define BORED_ENGINE_WINDOW_H

#include <memory>

#include "renderer.h"
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

	class WindowDelegate
	{
	public:
		virtual ~WindowDelegate();
		virtual void render(Window* window);
	};

	class Window : public RendererObject
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
		engine::Rect getRect();
		void setRect(engine::Rect const& rect); // set both position and size

		void setDelegate(WindowDelegate* delegate);

		void onRendererBackendChanged(RendererBackendType const& rendererBackendType) override;

		class WindowPlatformImplementation;

		WindowPlatformImplementation* getPlatformImplementation();
		WindowRendererImplementation* getRendererImplementation();

	private:

		std::unique_ptr<WindowPlatformImplementation> pPlatformImplementation;
		std::unique_ptr<WindowRendererImplementation> pRendererImplementation;

		WindowDelegate* pDelegate{nullptr};
	};
}

#endif //BORED_ENGINE_WINDOW_H
