//
// Created by Arjo Nagelhout on 21/12/2023.
//

#ifndef BORED_ENGINE_UIKIT_WINDOW_H
#define BORED_ENGINE_UIKIT_WINDOW_H

#include "graphics/window.h"

#import <Cocoa/Cocoa.h>

@interface WindowAdapter : NSWindow
@property (unsafe_unretained, nonatomic, nonnull) graphics::Window* pWindow;
@end

namespace graphics
{
	struct Window::Implementation
	{
		WindowAdapter* _Nonnull pWindowAdapter;
	};
}

#endif //BORED_ENGINE_UIKIT_WINDOW_H
