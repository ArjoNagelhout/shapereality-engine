//
// Created by Arjo Nagelhout on 29/01/2024.
//

#include "input.h"

using namespace graphics;

namespace input
{
    Input::Input() = default;

    Input::~Input() = default;

    void Input::onEvent(InputEvent const& event)
    {
        // store the state of key presses and mouse
        switch (event.getType())
        {
            case InputEventType::Mouse:
            {
                break;
            }
            case InputEventType::Keyboard:
            {
                KeyboardEvent const keyboard = event.getKeyboard();
                auto keyIndex = static_cast<size_t>(keyboard.keyCode);
                switch (keyboard.type)
                {
                    case KeyboardEventType::Up:
                    {
                        keys[keyIndex] = false;
                        break;
                    }
                    case KeyboardEventType::Down:
                    {
                        keys[keyIndex] = true;
                        break;
                    }
                    case KeyboardEventType::Repeat:
                    {
                        break; // do nothing when repeat
                    }
                    case KeyboardEventType::ModifiersChanged:
                    {
                        modifiers = keyboard.modifiers;
                    }
                    default: break;
                }
                break;
            }
            case InputEventType::None:
            case InputEventType::Scroll:
            case InputEventType::TextInput:
            case InputEventType::TextEditing: return;
        }
    }

    bool Input::getKey(KeyCode key) const
    {
        return keys[static_cast<size_t>(key)];
    }

    bool Input::getModifier(KeyboardModifier_ modifier) const
    {
        return (modifier & modifiers) != 0;
    }
}