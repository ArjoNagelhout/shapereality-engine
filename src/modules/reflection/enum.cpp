//
// Created by Arjo Nagelhout on 25/02/2024.
//

#include "enum.h"

#include <utility>

namespace reflection
{
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