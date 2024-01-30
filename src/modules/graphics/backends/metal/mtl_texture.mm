//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "mtl_texture.h"

#import <Metal/Metal.h>

#include "mtl_types.h"

#include <iostream>

namespace graphics
{
    MetalTexture::MetalTexture(id <MTLDevice> _Nonnull device, TextureDescriptor const& descriptor)
    {
        MTLTextureDescriptor* metalDescriptor = [[MTLTextureDescriptor alloc] init];
        metalDescriptor.width = descriptor.width;
        metalDescriptor.height = descriptor.height;
        metalDescriptor.pixelFormat = convert(descriptor.pixelFormat);
        metalDescriptor.textureType = MTLTextureType2D;
        metalDescriptor.usage = convert(descriptor.usage);
        metalDescriptor.arrayLength = 1;

        //metalDescriptor.mipmapLevelCount = 0;

        // todo: don't make these assumptions, just expose the entire texture API, including replaceRegion

        texture = [device newTextureWithDescriptor:metalDescriptor];
        [texture retain];

        if (descriptor.data != nullptr)
        {
            MTLRegion region = MTLRegionMake2D(0, 0, descriptor.width, descriptor.height);
            [texture replaceRegion:region
                      mipmapLevel:0 // if no mipmaps: use 0
                      slice:0 // for normal texture: use 0
                      withBytes:descriptor.data
                      bytesPerRow:descriptor.width * 4
                      bytesPerImage:0]; // only a single image: use 0
        }
    }

    MetalTexture::MetalTexture(id <MTLDrawable> _Nonnull _drawable)
    {
        drawable = _drawable;
        [drawable retain];
    }

    MetalTexture::MetalTexture(id <MTLTexture> _Nonnull _texture)
    {
        texture = _texture;
//        [pTexture retain];
    }

    MetalTexture::~MetalTexture()
    {
        if (drawable != nullptr)
        {
            [drawable release];
        }

        if (texture != nullptr)
        {
            [texture release];
        }
    }

    id <MTLTexture> MetalTexture::get() const
    {
        return texture;
    }

    id <MTLDrawable> MetalTexture::getDrawable() const
    {
        return drawable;
    }

    // ITexture interface

    TextureType MetalTexture::getTextureType() const
    {
        return convert(texture.textureType);
    }

    PixelFormat MetalTexture::getPixelFormat() const
    {
        return convert(texture.pixelFormat);
    }

    unsigned int MetalTexture::getWidth() const
    {
        return texture.width;
    }

    unsigned int MetalTexture::getHeight() const
    {
        return texture.height;
    }

    unsigned int MetalTexture::getDepth() const
    {
        return texture.depth;
    }

    unsigned int MetalTexture::getMipmapLevelCount() const
    {
        return texture.mipmapLevelCount;
    }

    unsigned int MetalTexture::getArrayLength() const
    {
        return texture.arrayLength;
    }

    std::uint8_t MetalTexture::getSampleCount() const
    {
        return texture.sampleCount;
    }

    bool MetalTexture::getIsFramebufferOnly() const
    {
        return texture.isFramebufferOnly;
    }

    TextureUsage_ MetalTexture::getUsage() const
    {
        return convertFromMetal(texture.usage);
    }
}