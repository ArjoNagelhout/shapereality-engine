//
// Created by Arjo Nagelhout on 18/12/2023.
//

#include "types.h"

#include <sstream>

namespace input
{
	std::string toString(MouseEvent const& event)
	{
		std::stringstream s{};
		s << toString(event.type);
		s << ", button: " << toString(event.mouseButton);
		s << ", x: " << event.x;
		s << ", y: " << event.y;
		return s.str();
	}

	std::string toString(ScrollEvent const& event)
	{
		std::stringstream s{};
		s << "x: " << event.x;
		s << ", y: " << event.y;
		return s.str();
	}

	std::string toString(KeyboardEvent const& event)
	{
		std::stringstream s{};
		s << toString(event.type);
		s << ", key: " << input::toString(event.keyCode);
		s << ", modifiers: " << input::toString(event.modifiers);
		return s.str();
	}

	std::string InputEvent::toString() const
	{
		std::stringstream s{};
		s << input::toString(type) << ": ";
		switch (type)
		{
			case InputEventType::Mouse:
				s << input::toString(mouse);
				break;
			case InputEventType::Scroll:
				s << input::toString(scroll);
				break;
			case InputEventType::Keyboard:
				s << input::toString(keyboard);
				break;
			case InputEventType::None:
				s << input::toString(InputEventType::None);
				break;
		}
		return s.str();
	}
}