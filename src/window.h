
#ifndef BORED_ENGINE_WINDOW_H
#define BORED_ENGINE_WINDOW_H

#include <memory>

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
		explicit Window(int const& x, int const& y, int const& width, int const& height, int const& flags);
		~Window();
		void setTitle(std::string const& title);

	private:
		struct Implementation;

		std::unique_ptr<Implementation> pImpl;
	};
}

#endif //BORED_ENGINE_WINDOW_H
