//
// Created by Arjo Nagelhout on 08/04/2024.
//

#include "enum.h"

namespace reflection
{
    EnumIterator::EnumIterator(EnumInfo const& info_, size_t index_) : info(info_), index(index_)
    {
        updateCurrent();
    }

    EnumIterator::const_reference EnumIterator::operator*() const
    {
        return current;
    }

    EnumIterator& EnumIterator::operator++()
    {
        index++;
        updateCurrent();
        return *this;
    }

    bool operator==(EnumIterator const& lhs, EnumIterator const& rhs)
    {
        return lhs.index == rhs.index;
    }

    bool operator!=(EnumIterator const& lhs, EnumIterator const& rhs)
    {
        return lhs.index != rhs.index;
    }

    void EnumIterator::updateCurrent()
    {
        if (index >= info.size())
        {
            return;
        }
        int value = info.cases[index];
        current = std::make_pair<int, std::string_view>(static_cast<int&&>(value), *info.to.at(value));
    }

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

    EnumIterator EnumInfo::begin() const
    {
        return EnumIterator{*this, 0};
    }

    EnumIterator EnumInfo::end() const
    {
        return EnumIterator{*this, cases.size()};
    }

    size_t EnumInfo::size() const
    {
        return cases.size();
    }

    bool EnumInfo::empty() const
    {
        return cases.empty();
    }

    void EnumInfo::updateToMap()
    {
        to.clear();
        for (auto& entry: from)
        {
            to[entry.second] = &entry.first;
        }
    }
}