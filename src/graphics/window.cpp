//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "window.h"

#include "backends/metal/mtl_window.h"

namespace graphics
{
	void IWindowDelegate::render(IWindow* window)
	{
	}

	IWindowDelegate* IWindow::getDelegate() const
	{
		return pDelegate;
	}

	void IWindow::setDelegate(graphics::IWindowDelegate* delegate)
	{
		pDelegate = delegate;
	}
}