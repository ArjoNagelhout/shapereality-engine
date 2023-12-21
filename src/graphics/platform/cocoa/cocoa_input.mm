//
// Created by Arjo Nagelhout on 17/12/2023.
//

#include "cocoa_input.h"

#include <iostream>

#include "cocoa.h"

namespace graphics
{
	KeyCode convert(unsigned short keyCode)
	{
		return KeyCode::None;
	}

	[[nodiscard]] KeyboardModifier_ convert(NSEventModifierFlags flags)
	{
		int mask = KeyboardModifier_None;
		mask |= (flags & NSEventModifierFlagCapsLock) ? KeyboardModifier_CapsLock : 0;
		mask |= (flags & NSEventModifierFlagShift) ? KeyboardModifier_Shift : 0;
		mask |= (flags & NSEventModifierFlagControl) ? KeyboardModifier_Control : 0;
		mask |= (flags & NSEventModifierFlagOption) ? KeyboardModifier_Option : 0;
		mask |= (flags & NSEventModifierFlagCommand) ? KeyboardModifier_Meta : 0;
		return static_cast<KeyboardModifier_>(mask);
	}

	InputEvent createMouseEvent(NSEvent* event, MouseEventType type, MouseButton mouseButton)
	{
		float x = 0.f;
		float y = 0.f;

		if (type != MouseEventType::Entered && type != MouseEventType::Exited)
		{
			x = static_cast<float>(event.locationInWindow.x);
			y = static_cast<float>(event.locationInWindow.y);
		}

		return InputEvent{
			.type = InputEventType::Mouse,
			.mouse = MouseEvent{
				.type = type,
				.mouseButton = mouseButton,
				.x = x,
				.y = y
			}
		};
	}

	InputEvent createScrollEvent(NSEvent* event)
	{
		return InputEvent{
			.type = InputEventType::Scroll,
			.scroll = ScrollEvent{
				.x = static_cast<float>(event.scrollingDeltaX),
				.y = static_cast<float>(event.scrollingDeltaY)
			}
		};
	}

	[[nodiscard]] InputEvent createKeyboardEvent(NSEvent* event, KeyboardEventType type)
	{
		KeyCode keyCode = KeyCode::None;
		std::array<char, 4> characters{};

		if (type != KeyboardEventType::ModifiersChanged)
		{
			if (event.isARepeat)
			{
				type = KeyboardEventType::Repeat;
			}

			// get the characters
			std::cout << toUtf8String(event.characters) << std::endl;
			std::cout << [event.characters lengthOfBytesUsingEncoding:NSUTF8StringEncoding] << std::endl;
			NSString* str = event.characters;
			NSData* data = [str dataUsingEncoding:NSUTF8StringEncoding];
			characters = {};
		}

		return InputEvent{
			.type = InputEventType::Keyboard,
			.keyboard = KeyboardEvent{
				.type = type,
				.keyCode = convert(event.keyCode),
				.modifiers = convert(event.modifierFlags)
			}
		};
	}
}