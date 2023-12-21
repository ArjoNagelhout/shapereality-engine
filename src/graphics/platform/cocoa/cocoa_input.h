//
// Created by Arjo Nagelhout on 18/12/2023.
//

#ifndef BORED_ENGINE_COCOA_INPUT_H
#define BORED_ENGINE_COCOA_INPUT_H

#include "../../input.h"

#include <memory>
#import <Cocoa/Cocoa.h>

namespace graphics
{
	// converts an NSEvent to an InputEvent
	[[nodiscard]] InputEvent convert(NSEvent* event);

	[[nodiscard]] KeyCode convert(unsigned short keyCode);
}

#endif //BORED_ENGINE_COCOA_INPUT_H
