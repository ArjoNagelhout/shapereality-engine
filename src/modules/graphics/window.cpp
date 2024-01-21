//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "window.h"

namespace graphics
{
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