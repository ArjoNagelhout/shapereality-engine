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
	//
	[[nodiscard]] KeyCode
	convert(unsigned short keyCode);

	//
	[[nodiscard]] KeyboardModifier_
	convert(NSEventModifierFlags flags);

	// helper function so that we don't have to repeat setting the cursor position and other properties
	[[nodiscard]] InputEvent
	createMouseEvent(NSEvent* event, MouseEventType type, MouseButton mouseButton = MouseButton::None);

	//
	[[nodiscard]] InputEvent
	createScrollEvent(NSEvent* event);

	//
	[[nodiscard]] InputEvent
	createKeyboardEvent(NSEvent* event, KeyboardEventType type);
}

#endif //BORED_ENGINE_COCOA_INPUT_H
