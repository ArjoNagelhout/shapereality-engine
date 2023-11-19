//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_WINDOW_COCOA_H
#define BORED_ENGINE_WINDOW_COCOA_H

#include "window.h"

#import <Cocoa/Cocoa.h>

namespace renderer
{
	struct Window::WindowPlatformImplementation
	{
		NSWindow* pWindow;
	};
}

#endif //BORED_ENGINE_WINDOW_COCOA_H
