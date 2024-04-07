//
// Created by Arjo Nagelhout on 15/02/2024.
//

#include "type_info.h"

#include <cassert>

namespace reflection
{
    TypeInfoRegistry::TypeInfoRegistry()
    {
        // built-in types
        emplace<bool>({.name = "bool"});
        emplace<int>({.name = "int"});
        emplace<float>({.name = "float"});
        emplace<double>({.name = "double"});
        emplace<std::string>({.name = "string"});
    }

    TypeInfoRegistry& TypeInfoRegistry::shared()
    {
        static TypeInfoRegistry instance_;
        return instance_;
    }

    void TypeInfoRegistry::emplace(TypeInfo&& info, TypeId typeId)
    {
        auto [entry, _] = types.emplace(typeId, std::move(info));
        TypeInfo& entryInfo = entry->second;
        if (entryInfo.base != kNullTypeId)
        {
            // update children of base type
            assert(types.contains(entryInfo.base) && "base types should be registered before child types");
            // todo: this could be mitigated in two ways:
            // 1. keep a record of all types that have base types registered, but of which the base type was not registered yet.
            //    these can then be added the moment the base type is registered.
            // 2. emplace an empty TypeInfo entry for the not yet registered base that already contains the children,
            //    but that is not "valid" yet, until the base type gets registered. this is cleaner.
            // however, this is not priority right now

            TypeInfo& base = types.at(entryInfo.base);
            base.children.emplace_back(typeId);
        }
    }

    bool TypeInfoRegistry::contains(TypeId typeId) const
    {
        return types.contains(typeId);
    }

    TypeInfo* TypeInfoRegistry::get(TypeId typeId)
    {
        if (types.contains(typeId))
        {
            return &(types.at(typeId));
        }
        else
        {
            return nullptr;
        }
    }

    TypeId TypeInfoRegistry::getChildType(std::any value, TypeId baseTypeId)
    {
        if (!types.contains(baseTypeId))
        {
            return kNullTypeId;
        }

        TypeInfo& base = types.at(baseTypeId);

        for (auto& childTypeId: base.children)
        {
            // for each child, check if it is that type, and if so, recurse
            TypeInfo& child = types.at(childTypeId);
            if (child.isType(value))
            {
                std::any childValue = child.castBaseTypeToThisType(value);
                return getChildType(childValue, childTypeId);
            }
        }

        // none of the children's isType (dynamic_cast) succeeded
        return baseTypeId;
    }
}