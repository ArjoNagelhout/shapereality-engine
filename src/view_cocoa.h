#ifndef BORED_ENGINE_VIEW_COCOA_H
#define BORED_ENGINE_VIEW_COCOA_H

#include "window.h"

#import <Cocoa/Cocoa.h>

namespace engine
{
	struct View::Implementation
	{
		NSView* pView;
	};
}

#endif //BORED_ENGINE_VIEW_COCOA_H
