//
// Created by Arjo Nagelhout on 25/02/2024.
//

#include "enum.h"

#include <utility>

namespace reflection
{
    Enum::Enum() = default;

    Enum::Enum(Enum const& rhs)
    {
        // todo: make sure we don't copy when it is not necessary
        from = rhs.from;
        functions = rhs.functions;
        rebuild();
    }

    void Enum::add(int key, std::string const& value)
    {
        from[value] = key;
    }

    void Enum::rebuild()
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

    void Enum::anyFromString(std::string const& in, std::any out) const
    {
        return functions.anyFromString(*this, in, std::move(out));
    }

    std::string Enum::anyToString(std::any in) const
    {
        assert(functions.anyToString && "function not registered");
        return functions.anyToString(*this, std::move(in));
    }

    void EnumSerializer::emplace(Enum&& e, TypeId typeId)
    {
        assert(!enums.contains(typeId) && "Enum type was already registered");
        enums.emplace(typeId, e);
    }

    bool EnumSerializer::contains(TypeId typeId) const
    {
        return enums.contains(typeId);
    }

    int EnumSerializer::fromString(std::string const& in, TypeId typeId)
    {
        assert(enums.contains(typeId) && "Enum type was not registered");
        return enums.at(typeId).fromString(in);
    }

    std::string EnumSerializer::toString(int in, TypeId typeId)
    {
        assert(enums.contains(typeId) && "Enum type was not registered");
        return enums.at(typeId).toString(in);
    }

    void EnumSerializer::anyFromString(std::string const& in, std::any out, TypeId typeId)
    {
        assert(enums.contains(typeId) && "Enum type was not registered");
        enums.at(typeId).anyFromString(in, out);
    }

    std::string EnumSerializer::anyToString(std::any in, TypeId typeId)
    {
        assert(enums.contains(typeId) && "Enum type was not registered");
        return enums.at(typeId).anyToString(std::move(in));
    }
}