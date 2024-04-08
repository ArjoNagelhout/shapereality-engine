//
// Created by Arjo Nagelhout on 08/04/2024.
//

#include "type_info_registry.h"

#include <reflection/type_info.h>
#include <cassert>
#include <reflection/class_info.h>

namespace reflection
{
    TypeInfoRegistry::TypeInfoRegistry()
    {
        // built-in types
        emplace<bool>(std::make_unique<PrimitiveInfo>("bool"));
        emplace<int>(std::make_unique<PrimitiveInfo>("int"));
        emplace<float>(std::make_unique<PrimitiveInfo>("float"));
        emplace<double>(std::make_unique<PrimitiveInfo>("double"));
        emplace<std::string>(std::make_unique<PrimitiveInfo>("string"));
    }

    TypeInfoRegistry::~TypeInfoRegistry() = default;

    TypeInfoRegistry& TypeInfoRegistry::shared()
    {
        static TypeInfoRegistry instance_;
        return instance_;
    }

    void TypeInfoRegistry::emplace(std::unique_ptr<TypeInfo>&& info, TypeId typeId)
    {
        auto [entry, _] = types.emplace(typeId, std::move(info));
        TypeInfo& entryInfo = *entry->second;

        if (entryInfo.type() == TypeInfo::Type::Class)
        {
            ClassInfo& classInfo = entryInfo.class_();
            if (classInfo.base)
            {
                // update children of base type
                assert(types.contains(classInfo.base) && "base types should be registered before child types");
                TypeInfo& base = *types.at(classInfo.base);

                assert(base.type() == TypeInfo::Type::Class && "base type should be of type Class");
                base.class_().children.emplace_back(typeId);
            }
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
            return types.at(typeId).get();
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
            return nullTypeId;
        }

        TypeInfo& base = *types.at(baseTypeId);
        assert(base.type() == TypeInfo::Type::Class);

        for (auto& childTypeId: base.class_().children)
        {
            // for each child, check if it is that type, and if so, recurse
            TypeInfo& child = *types.at(childTypeId);
            assert(child.type() == TypeInfo::Type::Class);
            if (child.class_().isType(value))
            {
                std::any childValue = child.class_().castBaseTypeToThisType(value);
                return getChildType(childValue, childTypeId);
            }
        }

        // none of the children's isType (dynamic_cast) succeeded
        return baseTypeId;
    }
}