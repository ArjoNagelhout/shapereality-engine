//
// Created by Arjo Nagelhout on 04/02/2024.
//

#ifndef SHAPEREALITY_TYPE_INFO_H
#define SHAPEREALITY_TYPE_INFO_H

#include "reflection_friend.h"
#include <reflection/type_id.h>
#include <reflection/type_info_registry.h>

#include <utility>
#include <unordered_map>
#include <vector>
#include <any>
#include <string>
#include <functional>
#include <cassert>

#include <iostream>

/**
 * @namespace reflection
 * @brief simple reflection of plain-old datastructures (POD) or compositions of PODs
 */
namespace reflection
{
    class TypeInfoRegistry;

    // Reflection system for inspecting C++ POD structs on runtime,
    // and serializing and deserializing to and from json using nlohmann's json library
    //
    // Supported:
    // - nested std::vector<T> and std::unordered_map<Key, Value>
    // - nested POD structs
    // - inheritance by using .base() in the ClassInfoBuilder
    //
    // Not supported and unplanned:
    // - inheritance
    // - function reflection
    //
    // Types and its properties can be registered using a ClassInfoBuilder or by manually
    // creating a TypeInfo, and then emplacing it in the TypeInfoRegistry.
    //
    // Internally, when registering a property, it uses recursive template instantiation
    // to create a hierarchy of TypeNodes:
    //
    // A std::vector<std::unordered_map<int, std::string>> property becomes
    // ListNode > DictionaryNode > ObjectNode (std::string).
    //
    // Each TypeInfo stores its own node hierarchy

    struct ClassInfo;
    struct EnumInfo;
    struct PrimitiveInfo;

    struct TypeInfo
    {
        enum class Type
        {
            Class, // class or struct
            Enum, // enum or enum class
            Primitive // uint32_t, int, any type that is easily convertable to and from a string
        };

        virtual ~TypeInfo();

        std::string name;

        [[nodiscard]] Type type();

        [[nodiscard]] ClassInfo& class_();

        [[nodiscard]] EnumInfo& enum_();

        [[nodiscard]] PrimitiveInfo& primitive();

    protected:
        explicit TypeInfo(std::string name, Type type);

        Type type_;
    };

    struct PrimitiveInfo final : public TypeInfo
    {
        explicit PrimitiveInfo(std::string name);

        std::string (* toString)(std::any) = nullptr;
    };
}

#endif //SHAPEREALITY_TYPE_INFO_H
