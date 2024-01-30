#include "application.h"

#include <iostream>

// common implementation between platforms

namespace graphics
{
    IApplicationDelegate* Application::getDelegate() const
    {
        return delegate;
    }

    void Application::setDelegate(IApplicationDelegate* _delegate)
    {
        delegate = _delegate;
    }
}