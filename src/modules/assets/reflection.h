//
// Created by Arjo Nagelhout on 04/02/2024.
//

#ifndef SHAPEREALITY_REFLECTION_H
#define SHAPEREALITY_REFLECTION_H

#include <string>
#include <unordered_map>
#include <utility>

namespace assets
{
    // reflection experiment

    using TypeId = size_t;

    struct TypeInfo
    {
        std::unordered_map<std::string, TypeId> properties;
    };

    // each type should be cached

    struct TypeInfoBuilder
    {
        TypeInfoBuilder& addProperty(std::string const& name, TypeId type)
        {
            typeInfo.properties[name] = type;
            return *this;
        }

        [[nodiscard]] TypeInfo build() const
        {
            return typeInfo;
        }

        TypeInfo typeInfo;
    };

    // contains a map of type ids to info's
    struct TypeRegistry
    {
        std::unordered_map<TypeId, TypeInfo> types;

        void registerType(TypeId type, TypeInfo info)
        {
            types[type] = std::move(info);
        }
    };
}

#endif //SHAPEREALITY_REFLECTION_H
