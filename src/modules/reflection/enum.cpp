//
// Created by Arjo Nagelhout on 25/02/2024.
//

#include "enum.h"

#include <utility>

namespace reflection
{
    Enum::Enum() = default;

    void Enum::add(int key, std::string const& value)
    {
        from[value] = key;
    }

    void Enum::build()
    {
        to.clear();
        for (auto& it: from)
        {
            to[it.second] = &it.first;
        }
    }

    std::string Enum::toString(int in) const
    {
        assert(to.contains(in));
        return *(to.at(in));
    }

    int Enum::fromString(std::string const& in) const
    {
        assert(from.contains(in));
        return from.at(in);
    }

    EnumSerializer::EnumSerializer() = default;

    EnumSerializer::~EnumSerializer() = default;

    void EnumSerializer::emplace(Enum&& e, type_id typeId)
    {
        assert(!enums.contains(typeId) && "Enum type was already registered");
        enums.emplace(typeId, e);
    }

    int EnumSerializer::fromString(std::string const& in, type_id typeId)
    {
        assert(enums.contains(typeId) && "Enum type was not registered");
        auto& e = enums.at(typeId);
        return e.fromString(in);
    }

    std::string EnumSerializer::toString(int in, type_id typeId)
    {
        assert(enums.contains(typeId) && "Enum type was not registered");
        auto& e = enums.at(typeId);
        return e.toString(in);
    }
}