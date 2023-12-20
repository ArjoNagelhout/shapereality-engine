//
// Created by Arjo Nagelhout on 17/12/2023.
//

#include "types_cocoa.h"

#include <iostream>

#include "common/platform/cocoa/cocoa.h"

namespace input
{
	// helper method so that we don't have to repeat setting the cursor position and other properties
	[[nodiscard]] InputEvent
	createMouseEvent(NSEvent* event, MouseEventType type, MouseButton mouseButton = MouseButton::None)
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

	[[nodiscard]] KeyCode convert(unsigned short keyCode)
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

	[[nodiscard]] InputEvent createKeyboardEvent(NSEvent* event, KeyboardEventType type)
	{
		KeyCode keyCode = KeyCode::None;

		if (type != KeyboardEventType::ModifiersChanged)
		{
			// get the characters
			std::cout << common::toUtf8String(event.characters) << std::endl;
		}

		return InputEvent{
			.type = InputEventType::Keyboard,
			.keyboard = KeyboardEvent{
				.type = type,
				.keyCode = convert(event.keyCode),
				.modifiers = convert(event.modifierFlags),
				//.textUtf8 = common::toUtf8String(event.characters)
			}
		};
	}

	[[nodiscard]] InputEvent convert(NSEvent* event)
	{
		NSEventType nsEventType = event.type;

		switch (nsEventType)
		{
			case NSEventTypeLeftMouseDown:
				return createMouseEvent(event, MouseEventType::Down, MouseButton::Left);
			case NSEventTypeLeftMouseUp:
				return createMouseEvent(event, MouseEventType::Up, MouseButton::Left);
			case NSEventTypeRightMouseDown:
				return createMouseEvent(event, MouseEventType::Down, MouseButton::Right);
			case NSEventTypeRightMouseUp:
				return createMouseEvent(event, MouseEventType::Up, MouseButton::Right);
			case NSEventTypeMouseMoved:
				return createMouseEvent(event, MouseEventType::Moved);
			case NSEventTypeLeftMouseDragged:
				return createMouseEvent(event, MouseEventType::Dragged, MouseButton::Left);
			case NSEventTypeRightMouseDragged:
				return createMouseEvent(event, MouseEventType::Dragged, MouseButton::Right);
			case NSEventTypeMouseEntered:
				return createMouseEvent(event, MouseEventType::Entered);
			case NSEventTypeMouseExited:
				return createMouseEvent(event, MouseEventType::Exited);
			case NSEventTypeKeyDown:
				return createKeyboardEvent(event, KeyboardEventType::Down);
			case NSEventTypeKeyUp:
				return createKeyboardEvent(event, KeyboardEventType::Up);
			case NSEventTypeScrollWheel:
				return createScrollEvent(event);
			case NSEventTypeOtherMouseDown:
				return createMouseEvent(event, MouseEventType::Down, MouseButton::Middle);
			case NSEventTypeOtherMouseUp:
				return createMouseEvent(event, MouseEventType::Up, MouseButton::Middle);
			case NSEventTypeOtherMouseDragged:
				return createMouseEvent(event, MouseEventType::Dragged, MouseButton::Middle);
			/* not handled (yet): */
			case NSEventTypeFlagsChanged:
				return createKeyboardEvent(event, KeyboardEventType::ModifiersChanged);
			case NSEventTypeAppKitDefined:
			case NSEventTypeSystemDefined:
			case NSEventTypeApplicationDefined:
			case NSEventTypePeriodic:
			case NSEventTypeCursorUpdate:
			case NSEventTypeTabletPoint:
			case NSEventTypeTabletProximity:
			case NSEventTypeGesture:
			case NSEventTypeMagnify:
			case NSEventTypeSwipe:
			case NSEventTypeRotate:
			case NSEventTypeBeginGesture:
			case NSEventTypeEndGesture:
			case NSEventTypeSmartMagnify:
			case NSEventTypeQuickLook:
			case NSEventTypePressure:
			case NSEventTypeDirectTouch:
			case NSEventTypeChangeMode:
				break;
		}

		// default return
		return InputEvent{
			.type = InputEventType::None
		};
	}
}