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
    };
}

#endif //SHAPEREALITY_TEXTURE_H
