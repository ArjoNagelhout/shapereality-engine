//
// Created by Arjo Nagelhout on 11/04/2024.
//

#include "register.h"

#include <reflection/enum.h>
#include <graphics/types.h>
#include <graphics/texture.h>

namespace graphics
{
    void register_(reflection::Reflection& reflection)
    {
        reflection::register_::Enum<PrimitiveType>("PrimitiveType")
            .case_(PrimitiveType::Point, "Point")
            .case_(PrimitiveType::Line, "Line")
            .case_(PrimitiveType::LineStrip, "LineStrip")
            .case_(PrimitiveType::Triangle, "Triangle")
            .case_(PrimitiveType::TriangleStrip, "TriangleStrip")
            .emplace(reflection.types);
    }

    void register_(asset::AssetTypeRegistry& assetTypes)
    {
        assetTypes.emplace<ITexture>(asset::AssetType{
            .fileExtension = "texture"
        });
    }
}