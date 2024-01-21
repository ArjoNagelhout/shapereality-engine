//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "mtl_texture.h"

#import <Metal/Metal.h>

#include "mtl_types.h"

#include <iostream>

namespace graphics
{
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

        // todo: don't make these assumptions, just expose the entire texture API, including replaceRegion

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

    MetalTexture::MetalTexture(id <MTLTexture> _Nonnull texture)
    {
        pTexture = texture;
//        [pTexture retain];
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

    // ITexture interface

    TextureType MetalTexture::getTextureType() const
    {
        return convert(pTexture.textureType);
    }

    PixelFormat MetalTexture::getPixelFormat() const
    {
        return convert(pTexture.pixelFormat);
    }

    unsigned int MetalTexture::getWidth() const
    {
        return pTexture.width;
    }

    unsigned int MetalTexture::getHeight() const
    {
        return pTexture.height;
    }

    unsigned int MetalTexture::getDepth() const
    {
        return pTexture.depth;
    }

    unsigned int MetalTexture::getMipmapLevelCount() const
    {
        return pTexture.mipmapLevelCount;
    }

    unsigned int MetalTexture::getArrayLength() const
    {
        return pTexture.arrayLength;
    }

    unsigned int MetalTexture::getSampleCount() const
    {
        return pTexture.sampleCount;
    }

    bool MetalTexture::getIsFramebufferOnly() const
    {
        return pTexture.isFramebufferOnly;
    }

    TextureUsage_ MetalTexture::getUsage() const
    {
        return convertFromMetal(pTexture.usage);
    }
}