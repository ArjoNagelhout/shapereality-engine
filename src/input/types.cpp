//
// Created by Arjo Nagelhout on 18/12/2023.
//

#include "types.h"

namespace input
{
	InputEvent::InputEvent(InputEventType _eventType)
		: eventType(_eventType)
	{

	}

	MouseInputEvent::MouseInputEvent(MouseEventType _mouseEventType, MouseButton _mouseButton)
		: InputEvent(InputEventType::Mouse),
		  mouseEventType(_mouseEventType),
		  mouseButton(_mouseButton)
	{

	}

	MouseInputEvent::~MouseInputEvent() = default;

	KeyboardInputEvent::KeyboardInputEvent(KeyboardEventType _keyboardEventType, Key _key)
		: InputEvent(InputEventType::Keyboard),
		  keyboardEventType(_keyboardEventType),
		  key(_key)
	{

	}

	KeyboardInputEvent::~KeyboardInputEvent() = default;
}