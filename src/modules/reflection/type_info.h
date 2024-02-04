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
    /*
     * The big question now is: how do we serialize data. We now have some basic
     * way to see which properties, of which types are present on a given type,
     * but we need to be able to "inspect" a given property and get or set its
     * data.
     *
     * Potential solutions:
     * - storing pointers, doing pointer arithmetic to calculate the positions in memory
     *   of specific properties
     * - storing offsetof(). I don't know how robust that is
     *
     * We don't want to store extra methods inside a struct.
     * That's a clear requirement, but we might actually have to do that.
     * Let's see how rttr does it. Because there you can have data types that
     * don't have macros inside of them to define functions for getting and setting
     * the given properties.
     * That was a wrong assumption, they do use macros inside the struct.
     *
     * Some extra requirements:
     * - we can assume any type we want to have reflected is recursively built up from a limited set of primitives,
     *   such as floats, ints and booleans.
     * - we want to have support for simple containers std::unordered_map (i.e. a dictionary)
     *   and std::vector (i.e. a list)
     * - we don't need any reflection of methods, as we don't want to create bindings. This is because the engine is a
     *   library, and the application is built in C++ as well.
     * - it is primarily used for json serialization, so a built-in way to serialize to and from json that recursively
     *   goes over all properties would be amazing
     * - for homogeneous data, we want to serialize to a binary file next to it, with a descriptor in json.
     *   this approach is taken by gltf and protobufs, and maybe we could include such functionality with a clear
     *   specification inside the reflection module as well.
     *
     *
     *
     * https://preshing.com/20180116/a-primitive-reflection-system-in-cpp-part-1/
     * https://github.com/preshing/FlexibleReflection/tree/part1
     *
     * https://isocpp.org/files/papers/n3996.pdf
     *
     */

    struct PropertyInfo
    {
        type_id type;
        size_t offset;
    };

    struct TypeInfo
    {
        std::string name;
        std::unordered_map<std::string, PropertyInfo> properties;
    };

    class TypeInfoBuilder
    {
    public:
        explicit TypeInfoBuilder(std::string name);

        template<typename Type>
        TypeInfoBuilder& addProperty(std::string const& name)
        {
            type_id id = TypeIndex<Type>().value();
            typeInfo.properties[name] = PropertyInfo{
                .type = id,
                .offset = offsetof(Type, name)
            };

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
