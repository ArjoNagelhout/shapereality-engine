//
// Created by Arjo Nagelhout on 24/02/2024.
//

#ifndef SHAPEREALITY_ENUM_H
#define SHAPEREALITY_ENUM_H

#include "type_info.h"

#include <cassert>
#include <iostream>

namespace reflection
{
    struct Enum final
    {
        using From = std::unordered_map<std::string, int>;
        From from;
        std::unordered_map<int, std::string const*> to;

        explicit Enum() = default;

        Enum(Enum const& rhs)
        {
            from = rhs.from;
            build();
        }

        // delete assignment operator
        Enum& operator=(Enum const&) = delete;

        Enum& add(int key, std::string const& value)
        {
            from[value] = key;
            return *this;
        }

        template<typename Type>
        Enum& add(Type key, std::string const& value)
        {
            return add(static_cast<int>(key), value);
        }

        // builds the "to" map
        Enum& build()
        {
            std::cout << "build called" << std::endl;
            for (auto& it: from)
            {
                to[it.second] = &it.first;
            }
            return *this;
        }

        [[nodiscard]] std::string toString(int in) const
        {
            assert(to.contains(in));
            return *(to.at(in));
        }

        [[nodiscard]] int fromString(std::string const& in) const
        {
            assert(from.contains(in));
            return from.at(in);
        }
    };

    /**
     * Converts enum from and to string
     */
    class EnumSerializer
    {
    public:
        explicit EnumSerializer();

        ~EnumSerializer();

        void emplace(Enum&& e, type_id typeId);

        template<typename Type>
        void emplace(Enum&& e)
        {
            type_id typeId = TypeIndex<Type>::value();
            emplace(std::forward<Enum>(e), typeId);
        }

        [[nodiscard]] int fromString(std::string const& in, type_id typeId);

        template<typename Type>
        [[nodiscard]] Type fromString(std::string const& in)
        {
            type_id typeId = TypeIndex<Type>::value();
            return static_cast<Type>(fromString(in, typeId));
        }

        [[nodiscard]] std::string toString(int in, type_id typeId);

        template<typename Type>
        [[nodiscard]] std::string toString(Type in)
        {
            type_id typeId = TypeIndex<Type>::value();
            return toString(static_cast<int>(in), typeId);
        }

    private:
        std::unordered_map<type_id, Enum> enums;
    };
}

#endif //SHAPEREALITY_ENUM_H
