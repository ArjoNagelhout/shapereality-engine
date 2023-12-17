//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "window.h"

#include <cassert>

namespace graphics
{
	void IWindowRenderDelegate::render(Window* window)
	{
	}

	IWindowRenderDelegate* Window::getRenderDelegate() const
	{
		return pDelegate;
	}

	std::unique_ptr<IRenderPass> Window::getRenderPass() const
	{
		assert(false && "interface class method should not be called ");
	}

	std::unique_ptr<ITexture> Window::getDrawable() const
	{
		assert(false && "interface class method should not be called ");
	}

	void Window::setRenderDelegate(IWindowRenderDelegate* delegate)
	{
		pDelegate = delegate;
	}

	Window::Implementation* Window::getImplementation() const
	{
		return pImplementation.get();
	}

	input::IInputDelegate* Window::getInputDelegate() const
	{
		return pInputDelegate;
	}

	void Window::setInputDelegate(input::IInputDelegate* inputDelegate)
	{
		pInputDelegate = inputDelegate;
	}
}