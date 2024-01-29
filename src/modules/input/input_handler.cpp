//
// Created by Arjo Nagelhout on 29/01/2024.
//

#include "input_handler.h"

using namespace graphics;

namespace input
{
    InputHandler::InputHandler() = default;

    InputHandler::~InputHandler() = default;

    void InputHandler::onEvent(InputEvent const& event)
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

    bool InputHandler::getKey(KeyCode key) const
    {
        return keys[static_cast<size_t>(key)];
    }

    bool InputHandler::getModifier(KeyboardModifier_ modifier) const
    {
        return (modifier & modifiers) != 0;
    }
}