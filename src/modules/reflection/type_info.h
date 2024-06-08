//
// Created by Arjo Nagelhout on 04/02/2024.
//

#ifndef SHAPEREALITY_TYPE_INFO_H
#define SHAPEREALITY_TYPE_INFO_H

#include <reflection/type_id.h>
#include <reflection/type_registry.h>

#include <utility>
#include <unordered_map>
#include <vector>
#include <any>
#include <string>
#include <functional>
#include <cassert>

#include <iostream>

namespace reflection
{
    class TypeRegistry;

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
