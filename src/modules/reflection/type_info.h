//
// Created by Arjo Nagelhout on 04/02/2024.
//

#ifndef SHAPEREALITY_TYPE_INFO_H
#define SHAPEREALITY_TYPE_INFO_H

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
        explicit TypeInfoBuilder(std::string name);

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
    class TypeInfoRegistry
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
                return {}; // error: type was not registered yet
            }
        }

    private:
        std::unordered_map<type_id, TypeInfo> types;
    };
}

#endif //SHAPEREALITY_TYPE_INFO_H
