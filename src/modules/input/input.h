//
// Created by Arjo Nagelhout on 29/01/2024.
//

#ifndef SHAPEREALITY_INPUT_INPUT_H
#define SHAPEREALITY_INPUT_INPUT_H

#include "graphics/input.h"

namespace input
{
    // `Input` is responsible for piping events and storing their current state,
    // as the graphics module is purely event based.
    // for polling and getting input from different input devices than a graphics::Window,
    // use an instance of the `Input` class
    class Input
    {
    public:
        explicit Input();

        ~Input();

        void onEvent(graphics::InputEvent const& event);

        // returns whether the provided key is pressed
        [[nodiscard]] bool getKey(graphics::KeyCode key) const;

        // returns whether the provided modifier key is pressed, these are also
        // available as individual keys (modifier = Meta, keys can be either KeyCode::LeftMeta or KeyCode::RightMeta)
        [[nodiscard]] bool getModifier(graphics::KeyboardModifier_ modifier) const;

    private:
        // state

        // key map
        std::bitset<static_cast<size_t>(graphics::KeyCode::Last)> keys{};

        // modifiers combine for example the left and right meta key
        graphics::KeyboardModifier_ modifiers = graphics::KeyboardModifier_None;
    };
}

#endif //SHAPEREALITY_INPUT_INPUT_H
