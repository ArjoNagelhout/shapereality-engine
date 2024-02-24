//
// Created by Arjo Nagelhout on 24/02/2024.
//

#ifndef SHAPEREALITY_ENUM_H
#define SHAPEREALITY_ENUM_H

#include "type_info.h"

#include <cassert>

namespace reflection
{
    template<typename UnderlyingType>
    struct Enum
    {
        using From = std::unordered_map<std::string, UnderlyingType>;
        From from;
        std::unordered_map<UnderlyingType, typename From::iterator> to;

        explicit Enum()
        {}

        Enum(Enum const& rhs)
        {
            from = rhs.from;
            build();
        }

        // delete assignment operator
        Enum& operator=(Enum const&) = delete;

        Enum& add(UnderlyingType key, std::string value)
        {
            from[value] = key;
            return *this;
        }

        // builds the "to" map
        Enum& build()
        {
            for (auto it = from.begin(); it != from.end(); it++)
            {
                to[it->second] = it;
            }
            return *this;
        }

        std::string toString(UnderlyingType in) const
        {
            assert(to.contains(in));
            return to.at(in)->first;
        }

        UnderlyingType fromString(std::string const& in) const
        {
            assert(from.contains(in));
            return from.at(in);
        }
    };

    /**
     *
     */
    class EnumSerializer
    {
    public:
        struct IEnum
        {

        };



        explicit EnumSerializer();

        ~EnumSerializer();

//        template<typename Type>
//        void emplace(IEnum&& f)
//        {
//            type_id typeId = TypeIndex<Type>::value();
//            assert(!enums.contains(typeId) && "already registered functions for type");
//            enums.emplace(typeId, f);
//        }
//
//        template<typename Type>
//        Type fromString(std::string const& in)
//        {
//
//        }
//
//        template<typename Type>
//        std::string toString(Type in)
//        {
//            type_id typeId = TypeIndex<Type>::value();
//            assert(enums.contains(typeId));
//        }
//
//    private:
//        std::unordered_map<type_id, Enum> enums;
    };
}

#endif //SHAPEREALITY_ENUM_H
