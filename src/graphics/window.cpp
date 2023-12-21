//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "window.h"

#include <cassert>

namespace graphics
{
	void IWindowRenderDelegate::render(Window* window)
	{
		assert(false && "interface class method should not be called ");
	}

	void IWindowInputDelegate::onEvent(const InputEvent& event, Window* window)
	{
		assert(false && "interface class method should not be called ");
	}

	IWindowRenderDelegate* Window::getRenderDelegate() const
	{
		return pRenderDelegate;
	}

	void Window::setRenderDelegate(IWindowRenderDelegate* renderDelegate)
	{
		pRenderDelegate = renderDelegate;
	}

	Window::Implementation* Window::getImplementation() const
	{
		return pImplementation.get();
	}

	IWindowInputDelegate* Window::getInputDelegate() const
	{
		return pInputDelegate;
	}

	void Window::setInputDelegate(IWindowInputDelegate* inputDelegate)
	{
		pInputDelegate = inputDelegate;
	}
}