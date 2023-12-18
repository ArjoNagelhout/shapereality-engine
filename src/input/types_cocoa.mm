//
// Created by Arjo Nagelhout on 17/12/2023.
//

#include "types_cocoa.h"

#include <iostream>

namespace input
{
	[[nodiscard]] InputEvent convert(NSEvent* event)
	{
		std::cout << event << std::endl;
		NSEventType nsEventType = event.type;

		MouseEvent mouseEvent;
		MouseButton mouseButton;

		KeyEvent keyEvent;
		//Key key = convert(event.keyCode);

		//NSString* characters = event.characters;

		switch (nsEventType)
		{
			case NSEventTypeLeftMouseDown:
				mouseEvent = MouseEvent::Down;
				mouseButton = MouseButton::Left;
				break;
			case NSEventTypeLeftMouseUp:
				mouseEvent = MouseEvent::Up;
				mouseButton = MouseButton::Left;
				break;
			case NSEventTypeRightMouseDown:
				mouseEvent = MouseEvent::Down;
				mouseButton = MouseButton::Right;
				break;
			case NSEventTypeRightMouseUp:
				mouseEvent = MouseEvent::Up;
				mouseButton = MouseButton::Right;
				break;
			case NSEventTypeMouseMoved:
				mouseEvent = MouseEvent::Moved;
				break;
			case NSEventTypeLeftMouseDragged:
				mouseEvent = MouseEvent::Dragged;
				mouseButton = MouseButton::Left;
				break;
			case NSEventTypeRightMouseDragged:
				mouseEvent = MouseEvent::Dragged;
				mouseButton = MouseButton::Right;
				break;
			case NSEventTypeMouseEntered:
				mouseEvent = MouseEvent::Entered;
				break;
			case NSEventTypeMouseExited:
				mouseEvent = MouseEvent::Exited;
				break;
			case NSEventTypeKeyDown:
				keyEvent = KeyEvent::Down;
				break;
			case NSEventTypeKeyUp:
				keyEvent = KeyEvent::Up;
				break;
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
				break;
			case NSEventTypeTabletPoint:
				break;
			case NSEventTypeTabletProximity:
				break;
			case NSEventTypeOtherMouseDown:
				break;
			case NSEventTypeOtherMouseUp:
				break;
			case NSEventTypeOtherMouseDragged:
				break;
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

		return InputEvent{};
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