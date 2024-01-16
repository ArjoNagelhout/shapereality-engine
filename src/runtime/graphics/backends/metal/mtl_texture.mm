//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "mtl_texture.h"

#import <Metal/Metal.h>

#include "mtl_types.h"

#include <iostream>

namespace graphics
{
    MTLTextureUsage convert(TextureUsage_ value)
    {
        MTLTextureUsage result{};
        result |= (value & TextureUsage_Unknown) ? MTLTextureUsageUnknown : 0;
        result |= (value & TextureUsage_ShaderRead) ? MTLTextureUsageShaderRead : 0;
        result |= (value & TextureUsage_ShaderWrite) ? MTLTextureUsageShaderWrite : 0;
        result |= (value & TextureUsage_ShaderAtomic) ? MTLTextureUsageShaderAtomic : 0;
        result |= (value & TextureUsage_RenderTarget) ? MTLTextureUsageRenderTarget : 0;
        return result;
    }

    MetalTexture::MetalTexture(id <MTLDevice> _Nonnull pDevice, TextureDescriptor const& descriptor)
    {
        MTLTextureDescriptor* metalDescriptor = [[MTLTextureDescriptor alloc] init];
        metalDescriptor.width = descriptor.width;
        metalDescriptor.height = descriptor.height;
        metalDescriptor.pixelFormat = convert(descriptor.pixelFormat);
        metalDescriptor.textureType = MTLTextureType2D;
        metalDescriptor.usage = convert(descriptor.usage);
        metalDescriptor.arrayLength = 1;

        //metalDescriptor.mipmapLevelCount = 0;

        pTexture = [pDevice newTextureWithDescriptor:metalDescriptor];
        [pTexture retain];

        if (descriptor.data != nullptr)
        {
            MTLRegion region = MTLRegionMake2D(0, 0, descriptor.width, descriptor.height);
            [pTexture replaceRegion:region
                mipmapLevel:0 // if no mipmaps: use 0
                slice:0 // for normal texture: use 0
                withBytes:descriptor.data
                bytesPerRow:descriptor.width * 4
                bytesPerImage:0]; // only a single image: use 0
        }
    }

    MetalTexture::MetalTexture(id <MTLDrawable> _Nonnull drawable)
    {
        pDrawable = drawable;
        [pDrawable retain];
    }

    MetalTexture::~MetalTexture()
    {
        if (pDrawable != nullptr)
        {
            [pDrawable release];
        }

        if (pTexture != nullptr)
        {
            [pTexture release];
        }
    }

    id <MTLTexture> MetalTexture::get() const
    {
        return pTexture;
    }

    id <MTLDrawable> MetalTexture::getDrawable() const
    {
        return pDrawable;
    }
}