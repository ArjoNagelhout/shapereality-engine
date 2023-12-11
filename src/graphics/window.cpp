//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "window.h"

#include <cassert>

namespace graphics
{
	void IWindowDelegate::render(Window* window)
	{
	}

	IWindowDelegate* Window::getDelegate() const
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

	void Window::setDelegate(graphics::IWindowDelegate* delegate)
	{
		pDelegate = delegate;
	}
}