//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef SHAPEREALITY_TEXTURE_H
#define SHAPEREALITY_TEXTURE_H

#include "graphics.h"

namespace graphics
{
    enum TextureUsage_ : int
    {
        TextureUsage_Unknown = 0,
        TextureUsage_ShaderRead = 1 << 0,
        TextureUsage_ShaderWrite = 1 << 1,
        TextureUsage_ShaderAtomic = 1 << 2,
        TextureUsage_RenderTarget = 1 << 3,
    };

    // "Type" prefix is so that we can have numeric identifiers
    enum class TextureType
    {
        Type1D,
        Type1DArray,
        Type2D,
        Type2DArray,
        Type2DMultisample,
        TypeCube,
        TypeCubeArray,
        Type3D,
        Type2DMultisampleArray,
        TypeTextureBuffer
    };

    struct TextureDescriptor
    {
        unsigned int width;
        unsigned int height;

        PixelFormat pixelFormat;
        TextureUsage_ usage;

        // data of the texture
        void const* data{nullptr};

        // size in bytes of the texture is calculated
        // from the width, height and pixel format,
        // so we don't have to specify it on texture creation

        // todo: add all texture properties
        // e.g. depth, storage mode, anti-aliasing
        // mip-mapping
    };

    class ITexture
    {
    public:
        virtual ~ITexture() = default;

        // querying texture attributes

        [[nodiscard]] virtual TextureType getTextureType() const = 0;

        [[nodiscard]] virtual PixelFormat getPixelFormat() const = 0;

        [[nodiscard]] virtual unsigned int getWidth() const = 0;

        [[nodiscard]] virtual unsigned int getHeight() const = 0;

        [[nodiscard]] virtual unsigned int getDepth() const = 0;

        [[nodiscard]] virtual unsigned int getMipmapLevelCount() const = 0;

        [[nodiscard]] virtual unsigned int getArrayLength() const = 0;

        [[nodiscard]] virtual unsigned int getSampleCount() const = 0;

        [[nodiscard]] virtual bool getIsFramebufferOnly() const = 0;

        [[nodiscard]] virtual TextureUsage_ getUsage() const = 0;
    };
}

#endif //SHAPEREALITY_TEXTURE_H
