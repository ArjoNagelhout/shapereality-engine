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
    class EnumSerializer;

    class Enum final
    {
    public:
        explicit Enum();

        Enum(Enum const& rhs);

        // delete assignment operator
        Enum& operator=(Enum const&) = delete;

        void add(int key, std::string const& value);

        // builds the "to" map
        void build();

        [[nodiscard]] std::string toString(int in) const;

        [[nodiscard]] int fromString(std::string const& in) const;

    private:
        std::unordered_map<std::string, int> from;
        std::unordered_map<int, std::string const*> to;
    };

    template<typename Type>
    class EnumBuilder
    {
    public:
        EnumBuilder& add(Type key, std::string const& value)
        {
            e.add(static_cast<int>(key), value);
            return *this;
        }

        void emplace(EnumSerializer& s);

    private:
        Enum e;
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

    template<typename Type>
    void EnumBuilder<Type>::emplace(EnumSerializer& s)
    {
        s.emplace<Type>(std::move(e));
    }
}

#endif //SHAPEREALITY_ENUM_H
