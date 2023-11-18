//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "mtl_texture.h"

namespace renderer
{
	MTL::PixelFormat fromTextureFormat(TextureFormat const& textureFormat)
	{
		switch (textureFormat)
		{
			case TextureFormat::Undefined:				return MTL::PixelFormatInvalid;
			case TextureFormat::A8Unorm:				return MTL::PixelFormatA8Unorm;
			case TextureFormat::R8Unorm:				return MTL::PixelFormatR8Unorm;
			case TextureFormat::R8Unorm_sRGB:			return MTL::PixelFormatR8Unorm_sRGB;
			case TextureFormat::R8Snorm:				return MTL::PixelFormatR8Snorm;
			case TextureFormat::R8Uint:					return MTL::PixelFormatR8Uint;
			case TextureFormat::R8Sint:					return MTL::PixelFormatR8Sint;
			case TextureFormat::R16Unorm:				return MTL::PixelFormatR16Unorm;
			case TextureFormat::R16Snorm:				return MTL::PixelFormatR16Snorm;
			case TextureFormat::R16Uint:				return MTL::PixelFormatR16Uint;
			case TextureFormat::R16Sint:				return MTL::PixelFormatR16Sint;
			case TextureFormat::R16Float:				return MTL::PixelFormatR16Float;
			case TextureFormat::RG8Unorm:				return MTL::PixelFormatRG8Unorm;
			case TextureFormat::RG8Unorm_sRGB:			return MTL::PixelFormatRG8Unorm_sRGB;
			case TextureFormat::RG8Snorm:				return MTL::PixelFormatRG8Snorm;
			case TextureFormat::RG8Uint:				return MTL::PixelFormatRG8Uint;
			case TextureFormat::RG8Sint:				return MTL::PixelFormatRG8Sint;
			case TextureFormat::B5G6R5Unorm:			return MTL::PixelFormatB5G6R5Unorm;
			case TextureFormat::A1BGR5Unorm:			return MTL::PixelFormatA1BGR5Unorm;
			case TextureFormat::ABGR4Unorm:				return MTL::PixelFormatABGR4Unorm;
			case TextureFormat::BGR5A1Unorm:			return MTL::PixelFormatBGR5A1Unorm;
			case TextureFormat::R32Uint:				return MTL::PixelFormatR32Uint;
			case TextureFormat::R32Sint:				return MTL::PixelFormatR32Sint;
			case TextureFormat::R32Float:				return MTL::PixelFormatR32Float;
			case TextureFormat::RG16Unorm:				return MTL::PixelFormatRG16Unorm;
			case TextureFormat::RG16Snorm:				return MTL::PixelFormatRG16Snorm;
			case TextureFormat::RG16Uint:				return MTL::PixelFormatRG16Uint;
			case TextureFormat::RG16Sint:				return MTL::PixelFormatRG16Sint;
			case TextureFormat::RG16Float:				return MTL::PixelFormatRG16Float;
			case TextureFormat::RGBA8Unorm:				return MTL::PixelFormatRGBA8Unorm;
			case TextureFormat::RGBA8Unorm_sRGB:		return MTL::PixelFormatRGBA8Unorm_sRGB;
			case TextureFormat::RGBA8Snorm:				return MTL::PixelFormatRGBA8Snorm;
			case TextureFormat::RGBA8Uint:				return MTL::PixelFormatRGBA8Uint;
			case TextureFormat::RGBA8Sint:				return MTL::PixelFormatRGBA8Sint;
			case TextureFormat::BGRA8Unorm:				return MTL::PixelFormatBGRA8Unorm;
			case TextureFormat::BGRA8Unorm_sRGB:		return MTL::PixelFormatBGRA8Unorm_sRGB;
			case TextureFormat::RGB10A2Unorm:			return MTL::PixelFormatRGB10A2Unorm;
			case TextureFormat::RGB10A2Uint:			return MTL::PixelFormatRGB10A2Uint;
			case TextureFormat::RG11B10Float:			return MTL::PixelFormatRG11B10Float;
			case TextureFormat::RGB9E5Float:			return MTL::PixelFormatRGB9E5Float;
			case TextureFormat::BGR10A2Unorm:			return MTL::PixelFormatBGR10A2Unorm;
			case TextureFormat::RG32Uint:				return MTL::PixelFormatRG32Uint;
			case TextureFormat::RG32Sint:				return MTL::PixelFormatRG32Sint;
			case TextureFormat::RG32Float:				return MTL::PixelFormatRG32Float;
			case TextureFormat::RGBA16Unorm:			return MTL::PixelFormatRGBA16Unorm;
			case TextureFormat::RGBA16Snorm:			return MTL::PixelFormatRGBA16Snorm;
			case TextureFormat::RGBA16Uint:				return MTL::PixelFormatRGBA16Uint;
			case TextureFormat::RGBA16Sint:				return MTL::PixelFormatRGBA16Sint;
			case TextureFormat::RGBA16Float:			return MTL::PixelFormatRGBA16Float;
			case TextureFormat::RGBA32Uint:				return MTL::PixelFormatRGBA32Uint;
			case TextureFormat::RGBA32Sint:				return MTL::PixelFormatRGBA32Sint;
			case TextureFormat::RGBA32Float:			return MTL::PixelFormatRGBA32Float;
			case TextureFormat::BC1_RGBA:				return MTL::PixelFormatBC1_RGBA;
			case TextureFormat::BC1_RGBA_sRGB:			return MTL::PixelFormatBC1_RGBA_sRGB;
			case TextureFormat::BC2_RGBA:				return MTL::PixelFormatBC2_RGBA;
			case TextureFormat::BC2_RGBA_sRGB:			return MTL::PixelFormatBC2_RGBA_sRGB;
			case TextureFormat::BC3_RGBA:				return MTL::PixelFormatBC3_RGBA;
			case TextureFormat::BC3_RGBA_sRGB:			return MTL::PixelFormatBC3_RGBA_sRGB;
			case TextureFormat::BC4_RUnorm:				return MTL::PixelFormatBC4_RUnorm;
			case TextureFormat::BC4_RSnorm:				return MTL::PixelFormatBC4_RSnorm;
			case TextureFormat::BC5_RGUnorm:			return MTL::PixelFormatBC5_RGUnorm;
			case TextureFormat::BC5_RGSnorm:			return MTL::PixelFormatBC5_RGSnorm;
			case TextureFormat::BC6H_RGBFloat:			return MTL::PixelFormatBC6H_RGBFloat;
			case TextureFormat::BC6H_RGBUfloat:			return MTL::PixelFormatBC6H_RGBUfloat;
			case TextureFormat::BC7_RGBAUnorm:			return MTL::PixelFormatBC7_RGBAUnorm;
			case TextureFormat::BC7_RGBAUnorm_sRGB:		return MTL::PixelFormatBC7_RGBAUnorm_sRGB;
			case TextureFormat::PVRTC_RGB_2BPP:			return MTL::PixelFormatPVRTC_RGB_2BPP;
			case TextureFormat::PVRTC_RGB_2BPP_sRGB:	return MTL::PixelFormatPVRTC_RGB_2BPP_sRGB;
			case TextureFormat::PVRTC_RGB_4BPP:			return MTL::PixelFormatPVRTC_RGB_4BPP;
			case TextureFormat::PVRTC_RGB_4BPP_sRGB:	return MTL::PixelFormatPVRTC_RGB_4BPP_sRGB;
			case TextureFormat::PVRTC_RGBA_2BPP:		return MTL::PixelFormatPVRTC_RGBA_2BPP;
			case TextureFormat::PVRTC_RGBA_2BPP_sRGB:	return MTL::PixelFormatPVRTC_RGBA_2BPP_sRGB;
			case TextureFormat::PVRTC_RGBA_4BPP:		return MTL::PixelFormatPVRTC_RGBA_4BPP;
			case TextureFormat::PVRTC_RGBA_4BPP_sRGB:	return MTL::PixelFormatPVRTC_RGBA_4BPP_sRGB;
			case TextureFormat::EAC_R11Unorm:			return MTL::PixelFormatEAC_R11Unorm;
			case TextureFormat::EAC_R11Snorm:			return MTL::PixelFormatEAC_R11Snorm;
			case TextureFormat::EAC_RG11Unorm:			return MTL::PixelFormatEAC_RG11Unorm;
			case TextureFormat::EAC_RG11Snorm:			return MTL::PixelFormatEAC_RG11Snorm;
			case TextureFormat::EAC_RGBA8:				return MTL::PixelFormatEAC_RGBA8;
			case TextureFormat::EAC_RGBA8_sRGB:			return MTL::PixelFormatEAC_RGBA8_sRGB;
			case TextureFormat::ETC2_RGB8:				return MTL::PixelFormatETC2_RGB8;
			case TextureFormat::ETC2_RGB8_sRGB:			return MTL::PixelFormatETC2_RGB8_sRGB;
			case TextureFormat::ETC2_RGB8A1:			return MTL::PixelFormatETC2_RGB8A1;
			case TextureFormat::ETC2_RGB8A1_sRGB:		return MTL::PixelFormatETC2_RGB8A1_sRGB;
			case TextureFormat::ASTC_4x4_sRGB:			return MTL::PixelFormatASTC_4x4_sRGB;
			case TextureFormat::ASTC_5x4_sRGB:			return MTL::PixelFormatASTC_5x4_sRGB;
			case TextureFormat::ASTC_5x5_sRGB:			return MTL::PixelFormatASTC_5x5_sRGB;
			case TextureFormat::ASTC_6x5_sRGB:			return MTL::PixelFormatASTC_6x5_sRGB;
			case TextureFormat::ASTC_6x6_sRGB:			return MTL::PixelFormatASTC_6x6_sRGB;
			case TextureFormat::ASTC_8x5_sRGB:			return MTL::PixelFormatASTC_8x5_sRGB;
			case TextureFormat::ASTC_8x6_sRGB:			return MTL::PixelFormatASTC_8x6_sRGB;
			case TextureFormat::ASTC_8x8_sRGB:			return MTL::PixelFormatASTC_8x8_sRGB;
			case TextureFormat::ASTC_10x5_sRGB:			return MTL::PixelFormatASTC_10x5_sRGB;
			case TextureFormat::ASTC_10x6_sRGB:			return MTL::PixelFormatASTC_10x6_sRGB;
			case TextureFormat::ASTC_10x8_sRGB:			return MTL::PixelFormatASTC_10x8_sRGB;
			case TextureFormat::ASTC_10x10_sRGB:		return MTL::PixelFormatASTC_10x10_sRGB;
			case TextureFormat::ASTC_12x10_sRGB:		return MTL::PixelFormatASTC_12x10_sRGB;
			case TextureFormat::ASTC_12x12_sRGB:		return MTL::PixelFormatASTC_12x12_sRGB;
			case TextureFormat::ASTC_4x4_LDR:			return MTL::PixelFormatASTC_4x4_LDR;
			case TextureFormat::ASTC_5x4_LDR:			return MTL::PixelFormatASTC_5x4_LDR;
			case TextureFormat::ASTC_5x5_LDR:			return MTL::PixelFormatASTC_5x5_LDR;
			case TextureFormat::ASTC_6x5_LDR:			return MTL::PixelFormatASTC_6x5_LDR;
			case TextureFormat::ASTC_6x6_LDR:			return MTL::PixelFormatASTC_6x6_LDR;
			case TextureFormat::ASTC_8x5_LDR:			return MTL::PixelFormatASTC_8x5_LDR;
			case TextureFormat::ASTC_8x6_LDR:			return MTL::PixelFormatASTC_8x6_LDR;
			case TextureFormat::ASTC_8x8_LDR:			return MTL::PixelFormatASTC_8x8_LDR;
			case TextureFormat::ASTC_10x5_LDR:			return MTL::PixelFormatASTC_10x5_LDR;
			case TextureFormat::ASTC_10x6_LDR:			return MTL::PixelFormatASTC_10x6_LDR;
			case TextureFormat::ASTC_10x8_LDR:			return MTL::PixelFormatASTC_10x8_LDR;
			case TextureFormat::ASTC_10x10_LDR:			return MTL::PixelFormatASTC_10x10_LDR;
			case TextureFormat::ASTC_12x10_LDR:			return MTL::PixelFormatASTC_12x10_LDR;
			case TextureFormat::ASTC_12x12_LDR:			return MTL::PixelFormatASTC_12x12_LDR;
			case TextureFormat::ASTC_4x4_HDR:			return MTL::PixelFormatASTC_4x4_HDR;
			case TextureFormat::ASTC_5x4_HDR:			return MTL::PixelFormatASTC_5x4_HDR;
			case TextureFormat::ASTC_5x5_HDR:			return MTL::PixelFormatASTC_5x5_HDR;
			case TextureFormat::ASTC_6x5_HDR:			return MTL::PixelFormatASTC_6x5_HDR;
			case TextureFormat::ASTC_6x6_HDR:			return MTL::PixelFormatASTC_6x6_HDR;
			case TextureFormat::ASTC_8x5_HDR:			return MTL::PixelFormatASTC_8x5_HDR;
			case TextureFormat::ASTC_8x6_HDR:			return MTL::PixelFormatASTC_8x6_HDR;
			case TextureFormat::ASTC_8x8_HDR:			return MTL::PixelFormatASTC_8x8_HDR;
			case TextureFormat::ASTC_10x5_HDR:			return MTL::PixelFormatASTC_10x5_HDR;
			case TextureFormat::ASTC_10x6_HDR:			return MTL::PixelFormatASTC_10x6_HDR;
			case TextureFormat::ASTC_10x8_HDR:			return MTL::PixelFormatASTC_10x8_HDR;
			case TextureFormat::ASTC_10x10_HDR:			return MTL::PixelFormatASTC_10x10_HDR;
			case TextureFormat::ASTC_12x10_HDR:			return MTL::PixelFormatASTC_12x10_HDR;
			case TextureFormat::ASTC_12x12_HDR:			return MTL::PixelFormatASTC_12x12_HDR;
			case TextureFormat::GBGR422:				return MTL::PixelFormatGBGR422;
			case TextureFormat::BGRG422:				return MTL::PixelFormatBGRG422;
			case TextureFormat::Depth16Unorm:			return MTL::PixelFormatDepth16Unorm;
			case TextureFormat::Depth32Float:			return MTL::PixelFormatDepth32Float;
			case TextureFormat::Stencil8:				return MTL::PixelFormatStencil8;
			case TextureFormat::Depth24Unorm_Stencil8:	return MTL::PixelFormatDepth24Unorm_Stencil8;
			case TextureFormat::Depth32Float_Stencil8:	return MTL::PixelFormatDepth32Float_Stencil8;
			case TextureFormat::X32_Stencil8:			return MTL::PixelFormatX32_Stencil8;
			case TextureFormat::X24_Stencil8:			return MTL::PixelFormatX24_Stencil8;
			case TextureFormat::BGRA10_XR:				return MTL::PixelFormatBGRA10_XR;
			case TextureFormat::BGRA10_XR_sRGB:			return MTL::PixelFormatBGRA10_XR_sRGB;
			case TextureFormat::BGR10_XR:				return MTL::PixelFormatBGR10_XR;
			case TextureFormat::BGR10_XR_sRGB:			return MTL::PixelFormatBGR10_XR_sRGB;
		}
	}

	MetalTexture::MetalTexture() : Texture(TextureFormat::Undefined)
	{

	}

	MetalTexture::~MetalTexture() = default;
}