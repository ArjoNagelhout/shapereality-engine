//
// Created by Arjo Nagelhout on 08/04/2024.
//

#include "type_registry.h"

#include <reflection/type_info.h>
#include <cassert>
#include <reflection/class.h>

namespace reflection
{
    TypeRegistry::TypeRegistry()
    {
        // built-in types todo: move to a place that makes more sense, rather than the constructor
        emplace<bool>(std::make_unique<PrimitiveInfo>("bool"));
        emplace<int>(std::make_unique<PrimitiveInfo>("int"));
        emplace<float>(std::make_unique<PrimitiveInfo>("float"));
        emplace<double>(std::make_unique<PrimitiveInfo>("double"));
        emplace<std::string>(std::make_unique<PrimitiveInfo>("string"));
    }

    void TypeRegistry::emplace(std::unique_ptr<TypeInfo>&& info, TypeId typeId)
    {
        auto [entry, _] = types.emplace(typeId, std::move(info));

        // update children of base type if needed
        TypeInfo& entryInfo = *entry->second;
        if (entryInfo.type() == TypeInfo::Type::Class)
        {
            ClassInfo& classInfo = entryInfo.class_();
            if (classInfo.base)
            {
                assert(types.contains(classInfo.base) && "base types should be registered before child types");
                TypeInfo& base = *types.at(classInfo.base);

                assert(base.type() == TypeInfo::Type::Class && "base type should be of type Class");
                base.class_().children.emplace_back(typeId);
            }
        }
    }

    bool TypeRegistry::contains(TypeId typeId) const
    {
        return types.contains(typeId);
    }

    TypeInfo& TypeRegistry::get(TypeId typeId)
    {
        assert(types.contains(typeId) && "registry does not contain provided typeId");
        return *types.at(typeId);
    }

    TypeId TypeRegistry::getChildType(std::any value, TypeId baseTypeId)
    {
        assert(types.contains(baseTypeId) && "registry does not contain provided typeId");

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