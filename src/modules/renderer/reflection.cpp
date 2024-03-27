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
        types.emplace<StorageType>({"StorageType"});

        reflection::EnumSerializer& enums = reflection::EnumSerializer::shared();
        reflection::EnumBuilder<StorageType>()
            .add(StorageType::SignedByte, "SignedByte")
            .add(StorageType::UnsignedByte, "UnsignedByte")
            .add(StorageType::SignedShort, "SignedShort")
            .add(StorageType::UnsignedShort, "UnsignedShort")
            .add(StorageType::UnsignedInt, "UnsignedInt")
            .add(StorageType::Float, "Float")
            .emplace(enums);

        reflection::EnumBuilder<VertexAttributeType_>()
            .add(VertexAttributeType_Position, "Position")
            .add(VertexAttributeType_Normal, "Normal")
            .add(VertexAttributeType_Tangent, "Tangent")
            .add(VertexAttributeType_TextureCoordinate, "TextureCoordinate")
            .add(VertexAttributeType_Color, "Color")
            .add(VertexAttributeType_Joints, "Joints")
            .add(VertexAttributeType_Weights, "Weights")
            .emplace(enums);
    }
}