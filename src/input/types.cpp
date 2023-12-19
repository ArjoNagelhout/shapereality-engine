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

		s << "eventType: " << MouseEvent::toString(event.type);
		s << " mouseButton: " << toString(event.mouseButton);

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
		return "keyboard";
	}

	std::string InputEvent::toString() const
	{
		std::stringstream s{};
		s << "InputEvent: { type:" << toString(type) << " { ";
		switch (type)
		{
			case Type::Mouse:
				s << input::toString(mouse);
				break;
			case Type::Scroll:
				s << input::toString(scroll);
				break;
			case Type::Keyboard:
				s << input::toString(keyboard);
				break;
			case Type::None:
				break;
		}
		s << " }}";
		return s.str();
	}
}