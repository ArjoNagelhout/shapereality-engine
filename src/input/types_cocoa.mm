//
// Created by Arjo Nagelhout on 17/12/2023.
//

#include "types_cocoa.h"

#include <iostream>

namespace input
{
	// helper method so that we don't have to repeat setting the cursor position and other properties
	[[nodiscard]] InputEvent
	createMouseEvent(NSEvent* event, MouseEvent::Type type, MouseButton mouseButton = MouseButton::None)
	{
		float x = 0.f;
		float y = 0.f;

		if (type != MouseEvent::Type::Entered && type != MouseEvent::Type::Exited)
		{
			x = static_cast<float>(event.absoluteX);
			y = static_cast<float>(event.absoluteY);
		}

		return {
			.type = InputEvent::Type::Mouse,
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
		return {
			.type = InputEvent::Type::Scroll,
			.scroll = ScrollEvent{
				.x = static_cast<float>(event.scrollingDeltaX),
				.y = static_cast<float>(event.scrollingDeltaY)
			}
		};
	}

	[[nodiscard]] InputEvent createKeyboardEvent(NSEvent* event, KeyboardEvent::Type type)
	{
		return {
			.type = InputEvent::Type::Keyboard,
			.keyboard = KeyboardEvent{
				.type = type,
			}
		};
	}

	[[nodiscard]] InputEvent convert(NSEvent* event)
	{
		NSEventType nsEventType = event.type;

		switch (nsEventType)
		{
			case NSEventTypeLeftMouseDown:
				return createMouseEvent(event, MouseEvent::Type::Down, MouseButton::Left);
			case NSEventTypeLeftMouseUp:
				return createMouseEvent(event, MouseEvent::Type::Up, MouseButton::Left);
			case NSEventTypeRightMouseDown:
				return createMouseEvent(event, MouseEvent::Type::Down, MouseButton::Right);
			case NSEventTypeRightMouseUp:
				return createMouseEvent(event, MouseEvent::Type::Up, MouseButton::Right);
			case NSEventTypeMouseMoved:
				return createMouseEvent(event, MouseEvent::Type::Moved);
			case NSEventTypeLeftMouseDragged:
				return createMouseEvent(event, MouseEvent::Type::Dragged, MouseButton::Left);
			case NSEventTypeRightMouseDragged:
				return createMouseEvent(event, MouseEvent::Type::Dragged, MouseButton::Right);
			case NSEventTypeMouseEntered:
				return createMouseEvent(event, MouseEvent::Type::Entered);
			case NSEventTypeMouseExited:
				return createMouseEvent(event, MouseEvent::Type::Exited);
			case NSEventTypeKeyDown:
				return createKeyboardEvent(event, KeyboardEvent::Type::Down);
			case NSEventTypeKeyUp:
				return createKeyboardEvent(event, KeyboardEvent::Type::Up);
				// todo: unhandled cases
			case NSEventTypeFlagsChanged:
				break;
			case NSEventTypeAppKitDefined:
				break;
			case NSEventTypeSystemDefined:
				break;
			case NSEventTypeApplicationDefined:
				break;
			case NSEventTypePeriodic:
				break;
			case NSEventTypeCursorUpdate:
				break;
			case NSEventTypeScrollWheel:
				return createScrollEvent(event);
			case NSEventTypeTabletPoint:
				break;
			case NSEventTypeTabletProximity:
				break;
			case NSEventTypeOtherMouseDown:
				return createMouseEvent(event, MouseEvent::Type::Down, MouseButton::Middle);
			case NSEventTypeOtherMouseUp:
				return createMouseEvent(event, MouseEvent::Type::Up, MouseButton::Middle);
			case NSEventTypeOtherMouseDragged:
				return createMouseEvent(event, MouseEvent::Type::Dragged, MouseButton::Middle);
			case NSEventTypeGesture:
				break;
			case NSEventTypeMagnify:
				break;
			case NSEventTypeSwipe:
				break;
			case NSEventTypeRotate:
				break;
			case NSEventTypeBeginGesture:
				break;
			case NSEventTypeEndGesture:
				break;
			case NSEventTypeSmartMagnify:
				break;
			case NSEventTypeQuickLook:
				break;
			case NSEventTypePressure:
				break;
			case NSEventTypeDirectTouch:
				break;
			case NSEventTypeChangeMode:
				break;
		}

		return InputEvent{
			.type = InputEvent::Type::None
		};
	}
}