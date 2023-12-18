//
// Created by Arjo Nagelhout on 17/12/2023.
//

#include "types_cocoa.h"

#include <iostream>

namespace input
{
	// helper method so that we don't have to repeat setting the cursor position and other properties
	[[nodiscard]] InputEvent createMouseInputEvent(NSEvent* event, MouseEventType mouseEventType, MouseButton mouseButton)
	{
		return MouseInputEvent(mouseEventType, mouseButton);
	}

	[[nodiscard]] InputEvent createKeyboardInputEvent(NSEvent* event, KeyboardEventType keyboardEventType)
	{
		return KeyboardInputEvent(keyboardEventType, Key::None);
	}

	[[nodiscard]] InputEvent convert(NSEvent* event)
	{
		std::cout << event << std::endl;
		NSEventType nsEventType = event.type;

		switch (nsEventType)
		{
			case NSEventTypeLeftMouseDown:
				return createMouseInputEvent(event, MouseEventType::Down, MouseButton::Left);
//			case NSEventTypeLeftMouseUp:
//				mouseEvent = MouseEventType::Up;
//				mouseButton = MouseButton::Left;
//				break;
//			case NSEventTypeRightMouseDown:
//				mouseEvent = MouseEventType::Down;
//				mouseButton = MouseButton::Right;
//				break;
//			case NSEventTypeRightMouseUp:
//				mouseEvent = MouseEventType::Up;
//				mouseButton = MouseButton::Right;
//				break;
//			case NSEventTypeMouseMoved:
//				mouseEvent = MouseEventType::Moved;
//				break;
//			case NSEventTypeLeftMouseDragged:
//				mouseEvent = MouseEventType::Dragged;
//				mouseButton = MouseButton::Left;
//				break;
//			case NSEventTypeRightMouseDragged:
//				mouseEvent = MouseEventType::Dragged;
//				mouseButton = MouseButton::Right;
//				break;
//			case NSEventTypeMouseEntered:
//				mouseEvent = MouseEventType::Entered;
//				break;
//			case NSEventTypeMouseExited:
//				mouseEvent = MouseEventType::Exited;
//				break;
//			case NSEventTypeKeyDown:
//				keyEvent = KeyboardEventType::Down;
//				break;
//			case NSEventTypeKeyUp:
//				keyEvent = KeyboardEventType::Up;
//				break;
//			case NSEventTypeFlagsChanged:
//				break;
//			case NSEventTypeAppKitDefined:
//				break;
//			case NSEventTypeSystemDefined:
//				break;
//			case NSEventTypeApplicationDefined:
//				break;
//			case NSEventTypePeriodic:
//				break;
//			case NSEventTypeCursorUpdate:
//				break;
//			case NSEventTypeScrollWheel:
//				break;
//			case NSEventTypeTabletPoint:
//				break;
//			case NSEventTypeTabletProximity:
//				break;
//			case NSEventTypeOtherMouseDown:
//				break;
//			case NSEventTypeOtherMouseUp:
//				break;
//			case NSEventTypeOtherMouseDragged:
//				break;
//			case NSEventTypeGesture:
//				break;
//			case NSEventTypeMagnify:
//				break;
//			case NSEventTypeSwipe:
//				break;
//			case NSEventTypeRotate:
//				break;
//			case NSEventTypeBeginGesture:
//				break;
//			case NSEventTypeEndGesture:
//				break;
//			case NSEventTypeSmartMagnify:
//				break;
//			case NSEventTypeQuickLook:
//				break;
//			case NSEventTypePressure:
//				break;
//			case NSEventTypeDirectTouch:
//				break;
//			case NSEventTypeChangeMode:
//				break;
		}

		return InputEvent{InputEventType::None};
	}

//	[[nodiscard]] Key convert(unsigned short keyCode)
//	{
//
//
//		if (keyCode == 0)
//		{
//
//		}
//	}
}