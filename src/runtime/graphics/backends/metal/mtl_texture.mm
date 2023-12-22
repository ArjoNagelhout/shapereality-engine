//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "mtl_texture.h"

#import <Metal/Metal.h>

#include "mtl_types.h"

#include <iostream>

namespace graphics
{
	MetalTexture::MetalTexture(TextureDescriptor descriptor)
	{

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