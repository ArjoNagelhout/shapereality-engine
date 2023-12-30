//
// Created by Arjo Nagelhout on 21/12/2023.
//

#include "input.h"

#include <sstream>

namespace graphics
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
        s << ", key: " << toString(event.keyCode);
        s << ", modifiers: " << toString(event.modifiers);
        return s.str();
    }

    std::string toString(TextInputEvent const& event)
    {
        std::stringstream s{};
        return s.str();
    }

    std::string toString(TextEditingEvent const& event)
    {
        std::stringstream s{};
        s << "composition: " << event.composition.data();
        s << "start: " << event.start;
        s << ", length: " << event.length;
        return s.str();
    }

    std::string InputEvent::toString() const
    {
        std::stringstream s{};
        s << graphics::toString(type) << ": ";
        switch (type)
        {
            case InputEventType::Mouse:
                s << graphics::toString(mouse);
                break;
            case InputEventType::Scroll:
                s << graphics::toString(scroll);
                break;
            case InputEventType::Keyboard:
                s << graphics::toString(keyboard);
                break;
            case InputEventType::TextInput:
                s << graphics::toString(textInput);
                break;
            case InputEventType::TextEditing:
                s << graphics::toString(textEditing);
                break;
            case InputEventType::None:
                s << graphics::toString(InputEventType::None);
                break;
        }
        return s.str();
    }
}