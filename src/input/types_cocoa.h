//
// Created by Arjo Nagelhout on 18/12/2023.
//

#ifndef BORED_ENGINE_TYPES_COCOA_H
#define BORED_ENGINE_TYPES_COCOA_H

#include "types.h"

#import <Cocoa/Cocoa.h>

namespace input
{
	[[nodiscard]] InputEvent convert(NSEvent* event);
}

#endif //BORED_ENGINE_TYPES_COCOA_H
