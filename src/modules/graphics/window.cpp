//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "window.h"

namespace graphics
{
    IWindowRenderDelegate* Window::getRenderDelegate() const
    {
        return renderDelegate;
    }

    void Window::setRenderDelegate(IWindowRenderDelegate* _renderDelegate)
    {
        renderDelegate = _renderDelegate;
    }

    Window::Implementation* Window::getImplementation() const
    {
        return implementation.get();
    }

    IWindowInputDelegate* Window::getInputDelegate() const
    {
        return inputDelegate;
    }

    void Window::setInputDelegate(IWindowInputDelegate* _inputDelegate)
    {
        inputDelegate = _inputDelegate;
    }
}