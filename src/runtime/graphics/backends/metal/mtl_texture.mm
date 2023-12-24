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

	MetalTexture::MetalTexture(id<MTLDevice> _Nonnull pDevice, TextureDescriptor const& descriptor)
	{
		MTLTextureDescriptor* metalDescriptor = [[MTLTextureDescriptor alloc] init];
		metalDescriptor.width = descriptor.width;
		metalDescriptor.height = descriptor.height;
		metalDescriptor.pixelFormat = convert(descriptor.pixelFormat),
		metalDescriptor.textureType = MTLTextureType2D;
		metalDescriptor.usage = convert(descriptor.usage);
		metalDescriptor.mipmapLevelCount = 1;

		pTexture = [pDevice newTextureWithDescriptor:metalDescriptor];
	}

	id<MTLDrawable> MetalTexture::getDrawable() const
	{
		return pDrawable;
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

	MetalTexture::MetalTexture(id<MTLDrawable> _Nonnull drawable)
	{
		pDrawable = drawable;
		[pDrawable retain];
	}

//	MetalTexture::MetalTexture(Texture* texture) : TextureImplementation(texture)
//	{
//		pImplementation = std::make_unique<Implementation>();
//
//		id<MTLDevice> pDevice = graphics::MetalGraphicsBackend::pInstance->getImplementation()->pDevice;
//
//		MTLTextureDescriptor* pTextureDescriptor = [[MTLTextureDescriptor alloc] init];
//		[pTextureDescriptor setWidth:texture->getWidth()];
//		[pTextureDescriptor setHeight:texture->getHeight()];
//		[pTextureDescriptor setPixelFormat:toMetalPixelFormat(texture->getFormat())];
//		[pTextureDescriptor setTextureType:MTLTextureType2D];
//		[pTextureDescriptor setStorageMode:MTLStorageModeManaged];
//		[pTextureDescriptor setUsage:MTLResourceUsageRead];
//
//		pImplementation->pTexture = [pDevice newTextureWithDescriptor:pTextureDescriptor];
//		[pTextureDescriptor release];
//
//		std::cout << "there's a texture" << std::endl;
//	}
//
//	MetalTexture::~MetalTexture()
//	{
//		[pImplementation->pTexture release];
//	}
}