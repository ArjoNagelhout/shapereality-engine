//
// Created by Arjo Nagelhout on 24/02/2024.
//

#include <gtest/gtest.h>

#include <reflection/serialize/json.h>

namespace enum_test
{
    template<typename UnderlyingType>
    struct EnumLookupTable
    {
        std::vector<std::string> toString;f
        std::unordered_map<std::string, UnderlyingType> fromString;
    };

    template<typename UnderlyingType>
    struct EnumBuilder
    {
        EnumBuilder& add(std::string key, UnderlyingType value)
        {
            lookupTable
            return *this;
        }

        EnumLookupTable<UnderlyingType> lookupTable;
    };

    TEST(Reflection, Enum)
    {

    }
}