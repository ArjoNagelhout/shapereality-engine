//
// Created by Arjo Nagelhout on 04/05/2024.
//

#ifndef SHAPEREALITY_ANY_UNORDERED_MAP_HANDLER_H
#define SHAPEREALITY_ANY_UNORDERED_MAP_HANDLER_H

#include <any>

namespace reflection
{
    namespace any_unordered_map_handler_implementation
    {
        enum class Action
        {
            Insert,
            At,
            Clear,
        };
    }

    template<typename Type>
    class AnyUnorderedMapHandler
    {
    public:
        using KeyType = Type::key_type;
        using MappedType = Type::mapped_type;
        using Action = any_unordered_map_handler_implementation::Action;

        [[nodiscard]] std::any handle(Action action, std::any& this_, std::any const& other)
        {
            switch (action)
            {
                case Action::Insert:
                {
                    break;
                }
                case Action::At:
                {
                    return at(this_, other);
                }
                case Action::Clear:
                {
                    clear(this_);
                    return nullptr;
                }
            }
        }

    private:
        [[nodiscard]] std::any at(std::any& this_, std::any const& other)
        {
            auto* t = std::any_cast<Type*>(this_);
            auto k = std::any_cast<KeyType>(other);
            return &((*t)[k]);
        }

        void clear(std::any& this_)
        {
            auto* t = std::any_cast<Type*>(this_);
            t->clear();
        }

        void addKey(std::any& this_, std::any const& other)
        {
            auto* t = std::any_cast<Type*>(this_);
            auto k = std::any_cast<KeyType>(other);

        }
    };
}

#endif //SHAPEREALITY_ANY_UNORDERED_MAP_HANDLER_H
