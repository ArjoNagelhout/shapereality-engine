//
// Created by Arjo Nagelhout on 04/05/2024.
//

#ifndef SHAPEREALITY_ANY_VECTOR_HANDLER_H
#define SHAPEREALITY_ANY_VECTOR_HANDLER_H

#include <any>

namespace reflection
{
    namespace any_vector_handler_implementation
    {
        enum class Action
        {
            Size,
            Resize,
            At
        };
    }

    /**
     *
     */
    template<typename Type>
    class AnyVectorHandler
    {
    public:
        using Action = any_vector_handler_implementation::Action;

        [[nodiscard]] std::any handle(Action action, std::any this_, std::any other)
        {
            switch (action)
            {
                case Action::Size:
                {
                    break;
                }
                case Action::Resize:
                {
                    break;
                }
                case Action::At:
                {
                    break;
                }
            }
        }

    private:


    };
}

#endif //SHAPEREALITY_ANY_VECTOR_HANDLER_H
