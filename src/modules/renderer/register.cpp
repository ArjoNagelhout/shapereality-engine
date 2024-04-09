//
// Created by Arjo Nagelhout on 27/03/2024.
//

#include "register.h"

#include <reflection/enum.h>
#include <renderer/mesh.h>

namespace renderer
{
    REGISTER_REFLECTION
    {
        reflection::EnumInfoBuilder<VertexAttribute_>("VertexAttribute")
            .case_(VertexAttribute_Position, "Position")
            .case_(VertexAttribute_Normal, "Normal")
            .case_(VertexAttribute_Tangent, "Tangent")
            .case_(VertexAttribute_TextureCoordinate, "TextureCoordinate")
            .case_(VertexAttribute_Color, "Color")
            .case_(VertexAttribute_Joints, "Joints")
            .case_(VertexAttribute_Weights, "Weights")
            .emplace(reflection.types);

        reflection::EnumInfoBuilder<ElementType>("ElementType")
            .case_(ElementType::Scalar, "Scalar")
            .case_(ElementType::Vector2, "Vector2")
            .case_(ElementType::Vector3, "Vector3")
            .case_(ElementType::Vector4, "Vector4")
            .case_(ElementType::Matrix2x2, "Matrix2x2")
            .case_(ElementType::Matrix3x3, "Matrix3x3")
            .case_(ElementType::Matrix4x4, "Matrix4x4")
            .emplace(reflection.types);

        reflection::EnumInfoBuilder<ComponentType>("ComponentType")
            .case_(ComponentType::SignedByte, "SignedByte")
            .case_(ComponentType::UnsignedByte, "UnsignedByte")
            .case_(ComponentType::SignedShort, "SignedShort")
            .case_(ComponentType::UnsignedShort, "UnsignedShort")
            .case_(ComponentType::UnsignedInt, "UnsignedInt")
            .case_(ComponentType::Float, "Float")
            .emplace(reflection.types);
    }

    REGISTER_ASSET_TYPE
    {
        assetTypes.emplace<Mesh_>(asset::AssetType{
            .fileExtension = "mesh"
        });
    };
}