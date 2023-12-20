//
// Created by Arjo Nagelhout on 18/12/2023.
//

#ifndef BORED_ENGINE_TYPES_COCOA_H
#define BORED_ENGINE_TYPES_COCOA_H

#include "types.h"

#include <memory>
#import <Cocoa/Cocoa.h>

namespace input
{
	// converts an NSEvent to an InputEvent
	[[nodiscard]] InputEvent convert(NSEvent* event);

	[[nodiscard]] KeyCode convert(unsigned short keyCode);
}

#endif //BORED_ENGINE_TYPES_COCOA_H
