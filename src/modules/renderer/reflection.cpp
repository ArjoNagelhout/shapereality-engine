//
// Created by Arjo Nagelhout on 27/03/2024.
//

#include "reflection.h"

#include <reflection/type_info.h>
#include <reflection/serialize/enum.h>
#include <renderer/mesh.h>

namespace renderer
{
    void registerReflection()
    {
        reflection::TypeInfoRegistry& types = reflection::TypeInfoRegistry::shared();
        types.emplace<ComponentType>({"ComponentType"});

        reflection::EnumSerializer& enums = reflection::EnumSerializer::shared();
        reflection::EnumBuilder<ComponentType>()
            .add(ComponentType::SignedByte, "SignedByte")
            .add(ComponentType::UnsignedByte, "UnsignedByte")
            .add(ComponentType::SignedShort, "SignedShort")
            .add(ComponentType::UnsignedShort, "UnsignedShort")
            .add(ComponentType::UnsignedInt, "UnsignedInt")
            .add(ComponentType::Float, "Float")
            .emplace(enums);
    }
}