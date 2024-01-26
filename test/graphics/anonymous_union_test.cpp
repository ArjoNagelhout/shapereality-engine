//
// Created by Arjo Nagelhout on 26/01/2024.
//

#include <string>
#include <iostream>
#include <utility>

#include "gtest/gtest.h"

enum class EventType
{
    Mouse,
    TextInput
};

struct MouseEvent
{
    float x;
    float y;
};

struct TextInputEvent
{
    std::string text;

    ~TextInputEvent()
    {

    }
};

struct Event
{
    EventType type;

    union
    {
        MouseEvent mouse;
        TextInputEvent textInput;
    };

    // copy constructor
    Event(Event const &event)
    {
        switch (event.type)
        {
            case EventType::Mouse:
                mouse = event.mouse;
                break;
            case EventType::TextInput:
                textInput = event.textInput;
                break;
        }
        type = event.type;
    }

    explicit Event(MouseEvent _mouse)
    {
        mouse = _mouse;
        type = EventType::Mouse;
    }

    explicit Event(const TextInputEvent& _inputEvent)
    {
        textInput = _inputEvent;
        type = EventType::TextInput;
    }

    ~Event()
    {
        if (type == EventType::TextInput)
        {
            textInput.~TextInputEvent();
        }
    }
};

Event getTestEvent()
{
    Event something = Event(
        TextInputEvent{
            .text = "Something amazing is happening here"
        }
    );

    return something;
}

TEST(AnonymousUnion, Test1)
{
    std::cout << "test 1" << std::endl;

    Event mouseEvent = Event(
        MouseEvent{
            .x = 50.f,
            .y = 100.f
        }
    );

    Event textInputEvent = getTestEvent();

    std::cout << textInputEvent.textInput.text << std::endl;
}