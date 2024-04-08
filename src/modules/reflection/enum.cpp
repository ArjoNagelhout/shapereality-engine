//
// Created by Arjo Nagelhout on 08/04/2024.
//

#include "enum.h"

namespace reflection
{
    EnumInfo::EnumInfo(std::string name_) : TypeInfo(std::move(name_), Type::Enum) {}

    void EnumInfo::anyFromString(std::string const& in, std::any out) const
    {
        assert(fromImplementation);
        fromImplementation(in, std::move(out), from);
    }

    std::string_view EnumInfo::anyToString(std::any in) const
    {
        assert(toImplementation);
        return toImplementation(std::move(in), to);
    }

    void EnumInfo::iterate(std::function<void(std::pair<int, std::string_view> const& a)> const& callback) const
    {
        for (auto& case_: cases)
        {
            callback(std::make_pair(case_, *to.at(case_)));
        }
    }

    void EnumInfo::buildToMap()
    {
        to.clear();
        for (auto& entry: from)
        {
            to[entry.second] = &entry.first;
        }
    }
}