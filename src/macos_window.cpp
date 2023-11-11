//
// Created by Arjo Nagelhout on 11/11/2023.
//

#include "window.h"

namespace engine
{
	Window::Window(int x, int y, int width, int height)
	{
		CGRect frame = (CGRect) {{static_cast<float>(x),  static_cast<float>(y)},
								 {static_cast<float>(width), static_cast<float>(height)}};

		pWindow = NS::Window::alloc()->init(
			frame,
			NS::WindowStyleMaskClosable | NS::WindowStyleMaskTitled,
			NS::BackingStoreBuffered,
			false);

		pWindow->makeKeyAndOrderFront(nullptr);
	}

	Window::~Window()
	{
		pWindow->release();
	}

	void Window::setTitle(const std::string& title)
	{
		pWindow->setTitle(NS::String::string(title.c_str(), NS::StringEncoding::UTF8StringEncoding));
	}
}