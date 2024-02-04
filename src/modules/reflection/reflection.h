//
// Created by Arjo Nagelhout on 04/02/2024.
//

#ifndef SHAPEREALITY_REFLECTION_H
#define SHAPEREALITY_REFLECTION_H

#include <string>
#include <unordered_map>
#include <utility>

#include "type_id.h"

namespace reflection
{
    struct TypeInfo
    {
        std::string name;
        std::unordered_map<std::string, type_id> properties;
    };

    class TypeInfoBuilder
    {
    public:
        template<typename Type>
        TypeInfoBuilder& addProperty(std::string const& name)
        {
            type_id id = TypeIndex<Type>().value();
            typeInfo.properties[name] = id;
            return *this;
        }

        [[nodiscard]] TypeInfo build() const;

    private:
        TypeInfo typeInfo;
    };

    // contains a map of type ids to info's
    class TypeRegistry
    {
    public:
        template<typename Type>
        void add(TypeInfo info)
        {
            type_id id = TypeIndex<Type>().value();
            types[id] = std::move(info);
        }

        template<typename Type>
        std::optional<TypeInfo> get()
        {
            type_id id = TypeIndex<Type>().value();
            if (types.contains(id))
            {
                return types[id];
            }
            else
            {
                return {}; // error:
            }
        }

    private:
        std::unordered_map<type_id, TypeInfo> types;
    };
}

#endif //SHAPEREALITY_REFLECTION_H
