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

        reflection::EnumSerializer& enums = reflection::EnumSerializer::shared();

        types.emplace<VertexAttribute_>(std::make_unique<reflection::EnumInfo>("VertexAttribute"));
        reflection::EnumBuilder<VertexAttribute_>()
            .add(VertexAttribute_Position, "Position")
            .add(VertexAttribute_Normal, "Normal")
            .add(VertexAttribute_Tangent, "Tangent")
            .add(VertexAttribute_TextureCoordinate, "TextureCoordinate")
            .add(VertexAttribute_Color, "Color")
            .add(VertexAttribute_Joints, "Joints")
            .add(VertexAttribute_Weights, "Weights")
            .emplace(enums);

        types.emplace<ElementType>(std::make_unique<reflection::EnumInfo>("ElementType"));
        reflection::EnumBuilder<ElementType>()
            .add(ElementType::Scalar, "Scalar")
            .add(ElementType::Vector2, "Vector2")
            .add(ElementType::Vector3, "Vector3")
            .add(ElementType::Vector4, "Vector4")
            .add(ElementType::Matrix2x2, "Matrix2x2")
            .add(ElementType::Matrix3x3, "Matrix3x3")
            .add(ElementType::Matrix4x4, "Matrix4x4")
            .emplace(enums);

        types.emplace<ComponentType>(std::make_unique<reflection::EnumInfo>("ComponentType"));
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