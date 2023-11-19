//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../mtl_texture.h"
#include "mtl_texture_implementation.h"
#include "mtl_renderer_implementation.h"

#import <Metal/Metal.h>

#include <iostream>

namespace renderer
{
	MTLPixelFormat toMetalTextureFormat(TextureFormat const& textureFormat)
	{
		switch (textureFormat)
		{
			case TextureFormat::Undefined:				return MTLPixelFormatInvalid;
			case TextureFormat::A8Unorm:				return MTLPixelFormatA8Unorm;
			case TextureFormat::R8Unorm:				return MTLPixelFormatR8Unorm;
			case TextureFormat::R8Unorm_sRGB:			return MTLPixelFormatR8Unorm_sRGB;
			case TextureFormat::R8Snorm:				return MTLPixelFormatR8Snorm;
			case TextureFormat::R8Uint:					return MTLPixelFormatR8Uint;
			case TextureFormat::R8Sint:					return MTLPixelFormatR8Sint;
			case TextureFormat::R16Unorm:				return MTLPixelFormatR16Unorm;
			case TextureFormat::R16Snorm:				return MTLPixelFormatR16Snorm;
			case TextureFormat::R16Uint:				return MTLPixelFormatR16Uint;
			case TextureFormat::R16Sint:				return MTLPixelFormatR16Sint;
			case TextureFormat::R16Float:				return MTLPixelFormatR16Float;
			case TextureFormat::RG8Unorm:				return MTLPixelFormatRG8Unorm;
			case TextureFormat::RG8Unorm_sRGB:			return MTLPixelFormatRG8Unorm_sRGB;
			case TextureFormat::RG8Snorm:				return MTLPixelFormatRG8Snorm;
			case TextureFormat::RG8Uint:				return MTLPixelFormatRG8Uint;
			case TextureFormat::RG8Sint:				return MTLPixelFormatRG8Sint;
			case TextureFormat::B5G6R5Unorm:			return MTLPixelFormatB5G6R5Unorm;
			case TextureFormat::A1BGR5Unorm:			return MTLPixelFormatA1BGR5Unorm;
			case TextureFormat::ABGR4Unorm:				return MTLPixelFormatABGR4Unorm;
			case TextureFormat::BGR5A1Unorm:			return MTLPixelFormatBGR5A1Unorm;
			case TextureFormat::R32Uint:				return MTLPixelFormatR32Uint;
			case TextureFormat::R32Sint:				return MTLPixelFormatR32Sint;
			case TextureFormat::R32Float:				return MTLPixelFormatR32Float;
			case TextureFormat::RG16Unorm:				return MTLPixelFormatRG16Unorm;
			case TextureFormat::RG16Snorm:				return MTLPixelFormatRG16Snorm;
			case TextureFormat::RG16Uint:				return MTLPixelFormatRG16Uint;
			case TextureFormat::RG16Sint:				return MTLPixelFormatRG16Sint;
			case TextureFormat::RG16Float:				return MTLPixelFormatRG16Float;
			case TextureFormat::RGBA8Unorm:				return MTLPixelFormatRGBA8Unorm;
			case TextureFormat::RGBA8Unorm_sRGB:		return MTLPixelFormatRGBA8Unorm_sRGB;
			case TextureFormat::RGBA8Snorm:				return MTLPixelFormatRGBA8Snorm;
			case TextureFormat::RGBA8Uint:				return MTLPixelFormatRGBA8Uint;
			case TextureFormat::RGBA8Sint:				return MTLPixelFormatRGBA8Sint;
			case TextureFormat::BGRA8Unorm:				return MTLPixelFormatBGRA8Unorm;
			case TextureFormat::BGRA8Unorm_sRGB:		return MTLPixelFormatBGRA8Unorm_sRGB;
			case TextureFormat::RGB10A2Unorm:			return MTLPixelFormatRGB10A2Unorm;
			case TextureFormat::RGB10A2Uint:			return MTLPixelFormatRGB10A2Uint;
			case TextureFormat::RG11B10Float:			return MTLPixelFormatRG11B10Float;
			case TextureFormat::RGB9E5Float:			return MTLPixelFormatRGB9E5Float;
			case TextureFormat::BGR10A2Unorm:			return MTLPixelFormatBGR10A2Unorm;
			case TextureFormat::RG32Uint:				return MTLPixelFormatRG32Uint;
			case TextureFormat::RG32Sint:				return MTLPixelFormatRG32Sint;
			case TextureFormat::RG32Float:				return MTLPixelFormatRG32Float;
			case TextureFormat::RGBA16Unorm:			return MTLPixelFormatRGBA16Unorm;
			case TextureFormat::RGBA16Snorm:			return MTLPixelFormatRGBA16Snorm;
			case TextureFormat::RGBA16Uint:				return MTLPixelFormatRGBA16Uint;
			case TextureFormat::RGBA16Sint:				return MTLPixelFormatRGBA16Sint;
			case TextureFormat::RGBA16Float:			return MTLPixelFormatRGBA16Float;
			case TextureFormat::RGBA32Uint:				return MTLPixelFormatRGBA32Uint;
			case TextureFormat::RGBA32Sint:				return MTLPixelFormatRGBA32Sint;
			case TextureFormat::RGBA32Float:			return MTLPixelFormatRGBA32Float;
			case TextureFormat::BC1_RGBA:				return MTLPixelFormatBC1_RGBA;
			case TextureFormat::BC1_RGBA_sRGB:			return MTLPixelFormatBC1_RGBA_sRGB;
			case TextureFormat::BC2_RGBA:				return MTLPixelFormatBC2_RGBA;
			case TextureFormat::BC2_RGBA_sRGB:			return MTLPixelFormatBC2_RGBA_sRGB;
			case TextureFormat::BC3_RGBA:				return MTLPixelFormatBC3_RGBA;
			case TextureFormat::BC3_RGBA_sRGB:			return MTLPixelFormatBC3_RGBA_sRGB;
			case TextureFormat::BC4_RUnorm:				return MTLPixelFormatBC4_RUnorm;
			case TextureFormat::BC4_RSnorm:				return MTLPixelFormatBC4_RSnorm;
			case TextureFormat::BC5_RGUnorm:			return MTLPixelFormatBC5_RGUnorm;
			case TextureFormat::BC5_RGSnorm:			return MTLPixelFormatBC5_RGSnorm;
			case TextureFormat::BC6H_RGBFloat:			return MTLPixelFormatBC6H_RGBFloat;
			case TextureFormat::BC6H_RGBUfloat:			return MTLPixelFormatBC6H_RGBUfloat;
			case TextureFormat::BC7_RGBAUnorm:			return MTLPixelFormatBC7_RGBAUnorm;
			case TextureFormat::BC7_RGBAUnorm_sRGB:		return MTLPixelFormatBC7_RGBAUnorm_sRGB;
			case TextureFormat::PVRTC_RGB_2BPP:			return MTLPixelFormatPVRTC_RGB_2BPP;
			case TextureFormat::PVRTC_RGB_2BPP_sRGB:	return MTLPixelFormatPVRTC_RGB_2BPP_sRGB;
			case TextureFormat::PVRTC_RGB_4BPP:			return MTLPixelFormatPVRTC_RGB_4BPP;
			case TextureFormat::PVRTC_RGB_4BPP_sRGB:	return MTLPixelFormatPVRTC_RGB_4BPP_sRGB;
			case TextureFormat::PVRTC_RGBA_2BPP:		return MTLPixelFormatPVRTC_RGBA_2BPP;
			case TextureFormat::PVRTC_RGBA_2BPP_sRGB:	return MTLPixelFormatPVRTC_RGBA_2BPP_sRGB;
			case TextureFormat::PVRTC_RGBA_4BPP:		return MTLPixelFormatPVRTC_RGBA_4BPP;
			case TextureFormat::PVRTC_RGBA_4BPP_sRGB:	return MTLPixelFormatPVRTC_RGBA_4BPP_sRGB;
			case TextureFormat::EAC_R11Unorm:			return MTLPixelFormatEAC_R11Unorm;
			case TextureFormat::EAC_R11Snorm:			return MTLPixelFormatEAC_R11Snorm;
			case TextureFormat::EAC_RG11Unorm:			return MTLPixelFormatEAC_RG11Unorm;
			case TextureFormat::EAC_RG11Snorm:			return MTLPixelFormatEAC_RG11Snorm;
			case TextureFormat::EAC_RGBA8:				return MTLPixelFormatEAC_RGBA8;
			case TextureFormat::EAC_RGBA8_sRGB:			return MTLPixelFormatEAC_RGBA8_sRGB;
			case TextureFormat::ETC2_RGB8:				return MTLPixelFormatETC2_RGB8;
			case TextureFormat::ETC2_RGB8_sRGB:			return MTLPixelFormatETC2_RGB8_sRGB;
			case TextureFormat::ETC2_RGB8A1:			return MTLPixelFormatETC2_RGB8A1;
			case TextureFormat::ETC2_RGB8A1_sRGB:		return MTLPixelFormatETC2_RGB8A1_sRGB;
			case TextureFormat::ASTC_4x4_sRGB:			return MTLPixelFormatASTC_4x4_sRGB;
			case TextureFormat::ASTC_5x4_sRGB:			return MTLPixelFormatASTC_5x4_sRGB;
			case TextureFormat::ASTC_5x5_sRGB:			return MTLPixelFormatASTC_5x5_sRGB;
			case TextureFormat::ASTC_6x5_sRGB:			return MTLPixelFormatASTC_6x5_sRGB;
			case TextureFormat::ASTC_6x6_sRGB:			return MTLPixelFormatASTC_6x6_sRGB;
			case TextureFormat::ASTC_8x5_sRGB:			return MTLPixelFormatASTC_8x5_sRGB;
			case TextureFormat::ASTC_8x6_sRGB:			return MTLPixelFormatASTC_8x6_sRGB;
			case TextureFormat::ASTC_8x8_sRGB:			return MTLPixelFormatASTC_8x8_sRGB;
			case TextureFormat::ASTC_10x5_sRGB:			return MTLPixelFormatASTC_10x5_sRGB;
			case TextureFormat::ASTC_10x6_sRGB:			return MTLPixelFormatASTC_10x6_sRGB;
			case TextureFormat::ASTC_10x8_sRGB:			return MTLPixelFormatASTC_10x8_sRGB;
			case TextureFormat::ASTC_10x10_sRGB:		return MTLPixelFormatASTC_10x10_sRGB;
			case TextureFormat::ASTC_12x10_sRGB:		return MTLPixelFormatASTC_12x10_sRGB;
			case TextureFormat::ASTC_12x12_sRGB:		return MTLPixelFormatASTC_12x12_sRGB;
			case TextureFormat::ASTC_4x4_LDR:			return MTLPixelFormatASTC_4x4_LDR;
			case TextureFormat::ASTC_5x4_LDR:			return MTLPixelFormatASTC_5x4_LDR;
			case TextureFormat::ASTC_5x5_LDR:			return MTLPixelFormatASTC_5x5_LDR;
			case TextureFormat::ASTC_6x5_LDR:			return MTLPixelFormatASTC_6x5_LDR;
			case TextureFormat::ASTC_6x6_LDR:			return MTLPixelFormatASTC_6x6_LDR;
			case TextureFormat::ASTC_8x5_LDR:			return MTLPixelFormatASTC_8x5_LDR;
			case TextureFormat::ASTC_8x6_LDR:			return MTLPixelFormatASTC_8x6_LDR;
			case TextureFormat::ASTC_8x8_LDR:			return MTLPixelFormatASTC_8x8_LDR;
			case TextureFormat::ASTC_10x5_LDR:			return MTLPixelFormatASTC_10x5_LDR;
			case TextureFormat::ASTC_10x6_LDR:			return MTLPixelFormatASTC_10x6_LDR;
			case TextureFormat::ASTC_10x8_LDR:			return MTLPixelFormatASTC_10x8_LDR;
			case TextureFormat::ASTC_10x10_LDR:			return MTLPixelFormatASTC_10x10_LDR;
			case TextureFormat::ASTC_12x10_LDR:			return MTLPixelFormatASTC_12x10_LDR;
			case TextureFormat::ASTC_12x12_LDR:			return MTLPixelFormatASTC_12x12_LDR;
			case TextureFormat::ASTC_4x4_HDR:			return MTLPixelFormatASTC_4x4_HDR;
			case TextureFormat::ASTC_5x4_HDR:			return MTLPixelFormatASTC_5x4_HDR;
			case TextureFormat::ASTC_5x5_HDR:			return MTLPixelFormatASTC_5x5_HDR;
			case TextureFormat::ASTC_6x5_HDR:			return MTLPixelFormatASTC_6x5_HDR;
			case TextureFormat::ASTC_6x6_HDR:			return MTLPixelFormatASTC_6x6_HDR;
			case TextureFormat::ASTC_8x5_HDR:			return MTLPixelFormatASTC_8x5_HDR;
			case TextureFormat::ASTC_8x6_HDR:			return MTLPixelFormatASTC_8x6_HDR;
			case TextureFormat::ASTC_8x8_HDR:			return MTLPixelFormatASTC_8x8_HDR;
			case TextureFormat::ASTC_10x5_HDR:			return MTLPixelFormatASTC_10x5_HDR;
			case TextureFormat::ASTC_10x6_HDR:			return MTLPixelFormatASTC_10x6_HDR;
			case TextureFormat::ASTC_10x8_HDR:			return MTLPixelFormatASTC_10x8_HDR;
			case TextureFormat::ASTC_10x10_HDR:			return MTLPixelFormatASTC_10x10_HDR;
			case TextureFormat::ASTC_12x10_HDR:			return MTLPixelFormatASTC_12x10_HDR;
			case TextureFormat::ASTC_12x12_HDR:			return MTLPixelFormatASTC_12x12_HDR;
			case TextureFormat::GBGR422:				return MTLPixelFormatGBGR422;
			case TextureFormat::BGRG422:				return MTLPixelFormatBGRG422;
			case TextureFormat::Depth16Unorm:			return MTLPixelFormatDepth16Unorm;
			case TextureFormat::Depth32Float:			return MTLPixelFormatDepth32Float;
			case TextureFormat::Stencil8:				return MTLPixelFormatStencil8;
			case TextureFormat::Depth24Unorm_Stencil8:	return MTLPixelFormatDepth24Unorm_Stencil8;
			case TextureFormat::Depth32Float_Stencil8:	return MTLPixelFormatDepth32Float_Stencil8;
			case TextureFormat::X32_Stencil8:			return MTLPixelFormatX32_Stencil8;
			case TextureFormat::X24_Stencil8:			return MTLPixelFormatX24_Stencil8;
			case TextureFormat::BGRA10_XR:				return MTLPixelFormatBGRA10_XR;
			case TextureFormat::BGRA10_XR_sRGB:			return MTLPixelFormatBGRA10_XR_sRGB;
			case TextureFormat::BGR10_XR:				return MTLPixelFormatBGR10_XR;
			case TextureFormat::BGR10_XR_sRGB:			return MTLPixelFormatBGR10_XR_sRGB;
		}
	}

	MetalTextureImplementation::MetalTextureImplementation(Texture* texture) : TextureImplementation(texture)
	{
		pImplementation = std::make_unique<Implementation>();

		id<MTLDevice> pDevice = renderer::MetalRendererBackend::pInstance->getImplementation()->pDevice;

		MTLTextureDescriptor* pTextureDescriptor = [[MTLTextureDescriptor alloc] init];
		[pTextureDescriptor setWidth:texture->getWidth()];
		[pTextureDescriptor setHeight:texture->getHeight()];
		[pTextureDescriptor setPixelFormat:toMetalTextureFormat(texture->getFormat())];
		[pTextureDescriptor setTextureType:MTLTextureType2D];
		[pTextureDescriptor setStorageMode:MTLStorageModeManaged];
		[pTextureDescriptor setUsage:MTLResourceUsageRead | MTLResourceUsageSample];

		pImplementation->pTexture = [pDevice newTextureWithDescriptor:pTextureDescriptor];
		[pTextureDescriptor release];

		std::cout << "there's a texture" << std::endl;
	}

	MetalTextureImplementation::~MetalTextureImplementation()
	{
		[pImplementation->pTexture release];
	}
}