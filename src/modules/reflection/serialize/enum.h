//
// Created by Arjo Nagelhout on 24/02/2024.
//

#ifndef SHAPEREALITY_ENUM_H
#define SHAPEREALITY_ENUM_H

#include "reflection/type_info.h"

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

        // add enum case
        void add(int key, std::string const& value);

        // every time Enum gets copied, pointers inside the "to" map get invalidated,
        // so we need to rebuild the map
        void rebuild();

        [[nodiscard]] std::string toString(int in) const;

        [[nodiscard]] int fromString(std::string const& in) const;

        // any

        void anyFromString(std::string const& in, std::any out) const;

        std::string anyToString(std::any in) const;

        template<typename Type>
        void addFunctions()
        {
            functions = {
                .anyFromString = [](Enum const& e, std::string const& in, std::any out) {
                    *std::any_cast<Type*>(out) = static_cast<Type>(e.fromString(in));
                },
                .anyToString = [](Enum const& e, std::any in) -> std::string {
                    return e.toString(static_cast<int>(*std::any_cast<Type*>(in)));
                }
            };
        }

    private:
        // functions to convert from and to any
        // in order to support copying the lambdas on copying Enum, we pass
        // Enum as an argument to the lambda, instead of capturing "this".
        struct Functions
        {
            std::function<void(Enum const& e, std::string const& in, std::any out)> anyFromString;
            std::function<std::string(Enum const& e, std::any in)> anyToString;
        };

        std::unordered_map<std::string, int> from;
        std::unordered_map<int, std::string const*> to;
        Functions functions;
    };

    template<typename Type>
    class EnumBuilder
    {
    public:
        EnumBuilder& add(Type key, std::string const& value)
        {
            // maybe assert that the key is inside the range of int?
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
            emplace(std::move(e), typeId);
        }

        [[nodiscard]] bool contains(type_id typeId) const;

        template<typename Type>
        [[nodiscard]] bool contains() const
        {
            type_id typeId = TypeIndex<Type>::value();
            return contains(typeId);
        }

        // from string

        [[nodiscard]] int fromString(std::string const& in, type_id typeId);

        template<typename Type>
        [[nodiscard]] Type fromString(std::string const& in)
        {
            type_id typeId = TypeIndex<Type>::value();
            return static_cast<Type>(fromString(in, typeId));
        }

        // to string

        [[nodiscard]] std::string toString(int in, type_id typeId);

        template<typename Type>
        [[nodiscard]] std::string toString(Type in)
        {
            type_id typeId = TypeIndex<Type>::value();
            return toString(static_cast<int>(in), typeId);
        }

        // any

        void anyFromString(std::string const& in, std::any out, type_id typeId);

        [[nodiscard]] std::string anyToString(std::any in, type_id typeId);

    private:
        std::unordered_map<type_id, Enum> enums;
    };

    template<typename Type>
    void EnumBuilder<Type>::emplace(EnumSerializer& s)
    {
        e.rebuild(); // rebuild "to" map before moving
        e.addFunctions<Type>();
        s.emplace<Type>(std::move(e));
    }
}

#endif //SHAPEREALITY_ENUM_H
