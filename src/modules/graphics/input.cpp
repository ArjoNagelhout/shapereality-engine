//
// Created by Arjo Nagelhout on 21/12/2023.
//

#include <graphics/input.h>

#include <sstream>
#include <cassert>

namespace graphics
{
    std::string string(MouseEvent const& event)
    {
        std::stringstream s{};
        s << string(event.type);
        s << ", button: " << string(event.mouseButton);
        s << ", x: " << event.x;
        s << ", y: " << event.y;
        return s.str();
    }

    std::string string(ScrollEvent const& event)
    {
        std::stringstream s{};
        s << "x: " << event.x;
        s << ", y: " << event.y;
        return s.str();
    }

    std::string string(KeyboardEvent const& event)
    {
        std::stringstream s{};
        s << string(event.type);
        s << ", key: " << string(event.keyCode);
        s << ", modifiers: " << string(event.modifiers);
        return s.str();
    }

    std::string string(TextInputEvent const& event)
    {
        std::stringstream s{};
        return s.str();
    }

    std::string string(TextEditingEvent const& event)
    {
        std::stringstream s{};
        s << "composition: " << event.composition.data();
        s << "start: " << event.start;
        s << ", length: " << event.length;
        return s.str();
    }

    InputEvent::InputEvent(MouseEvent _mouse)
    {
        mouse = _mouse;
        type = InputEventType::Mouse;
    }

    InputEvent::InputEvent(ScrollEvent _scroll)
    {
        scroll = _scroll;
        type = InputEventType::Scroll;
    }

    InputEvent::InputEvent(KeyboardEvent _keyboard)
    {
        keyboard = _keyboard;
        type = InputEventType::Keyboard;
    }

    InputEvent::InputEvent(TextInputEvent _textInput)
    {
        textInput = std::move(_textInput);
        type = InputEventType::TextInput;
    }

    InputEvent::InputEvent(TextEditingEvent _textEditing)
    {
        textEditing = std::move(_textEditing);
        type = InputEventType::TextEditing;
    }

    InputEvent::~InputEvent()
    {
        if (type == InputEventType::TextEditing)
        {
            textEditing.~TextEditingEvent();
        }
        else if (type == InputEventType::TextInput)
        {
            textInput.~TextInputEvent();
        }
    }

    InputEventType InputEvent::getType() const
    {
        return type;
    }

    MouseEvent InputEvent::getMouse() const
    {
        assert(type == InputEventType::Mouse);
        return mouse;
    }

    ScrollEvent InputEvent::getScroll() const
    {
        assert(type == InputEventType::Scroll);
        return scroll;
    }

    KeyboardEvent InputEvent::getKeyboard() const
    {
        assert(type == InputEventType::Keyboard);
        return keyboard;
    }

    TextInputEvent InputEvent::getTextInput() const
    {
        assert(type == InputEventType::TextInput);
        return textInput;
    }

    TextEditingEvent InputEvent::getTextEditing() const
    {
        assert(type == InputEventType::TextEditing);
        return textEditing;
    }

    std::string InputEvent::string() const
    {
        std::stringstream s{};
        s << graphics::string(type) << ": ";
        switch (type)
        {
            case InputEventType::Mouse:s << graphics::string(mouse);
                break;
            case InputEventType::Scroll:s << graphics::string(scroll);
                break;
            case InputEventType::Keyboard:s << graphics::string(keyboard);
                break;
            case InputEventType::TextInput:s << graphics::string(textInput);
                break;
            case InputEventType::TextEditing:s << graphics::string(textEditing);
                break;
            case InputEventType::None:s << graphics::string(InputEventType::None);
                break;
        }
        return s.str();
    }
}