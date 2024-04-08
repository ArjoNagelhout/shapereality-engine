//
// Created by Arjo Nagelhout on 15/02/2024.
//

#include "type_info.h"

#include "class_info.h"

namespace reflection
{
    TypeInfo::~TypeInfo() = default;

    TypeInfo::Type TypeInfo::type()
    {
        return type_;
    }

    ClassInfo& TypeInfo::class_()
    {
        assert(type_ == Type::Class);
        return *static_cast<ClassInfo*>(this);
    }

    EnumInfo& TypeInfo::enum_()
    {
        assert(type_ == Type::Enum);
        return *static_cast<EnumInfo*>(this);
    }

    PrimitiveInfo& TypeInfo::primitive()
    {
        assert(type_ == Type::Primitive);
        return *static_cast<PrimitiveInfo*>(this);
    }

    TypeInfo::TypeInfo(std::string name_, Type type)
        : name(std::move(name_)), type_(type) {}

    ClassInfo::ClassInfo(std::string name_)
        : TypeInfo(std::move(name_), Type::Class) {}

    EnumInfo::EnumInfo(std::string name_)
        : TypeInfo(std::move(name_), Type::Enum) {}

    PrimitiveInfo::PrimitiveInfo(std::string name_)
        : TypeInfo(std::move(name_), Type::Primitive) {}
}