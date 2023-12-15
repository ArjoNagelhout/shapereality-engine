//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_types.h"

namespace graphics
{
	MTLWinding convert(WindingOrder windingOrder)
	{
		switch (windingOrder)
		{
			case WindingOrder::Clockwise: return MTLWindingClockwise;
			case WindingOrder::CounterClockwise: return MTLWindingCounterClockwise;
		}
	}

	MTLPrimitiveType convert(PrimitiveType primitiveType)
	{
		switch (primitiveType)
		{
			case PrimitiveType::Point: return MTLPrimitiveTypePoint;
			case PrimitiveType::Line: return MTLPrimitiveTypeLine;
			case PrimitiveType::LineStrip: return MTLPrimitiveTypeLineStrip;
			case PrimitiveType::Triangle: return MTLPrimitiveTypeTriangle;
			case PrimitiveType::TriangleStrip: return MTLPrimitiveTypeTriangleStrip;
		}
	}

	MTLCullMode convert(CullMode cullMode)
	{
		switch (cullMode)
		{
			case CullMode::None: return MTLCullModeNone;
			case CullMode::Front: return MTLCullModeFront;
			case CullMode::Back: return MTLCullModeBack;
		}
	}

	MTLCompareFunction convert(CompareFunction compareFunction)
	{
		switch (compareFunction)
		{
			case CompareFunction::Never: return MTLCompareFunctionNever;
			case CompareFunction::Less: return MTLCompareFunctionLess;
			case CompareFunction::Equal: return MTLCompareFunctionEqual;
			case CompareFunction::LessEqual: return MTLCompareFunctionLessEqual;
			case CompareFunction::Greater: return MTLCompareFunctionGreater;
			case CompareFunction::NotEqual: return MTLCompareFunctionNotEqual;
			case CompareFunction::GreaterEqual: return MTLCompareFunctionGreaterEqual;
			case CompareFunction::Always: return MTLCompareFunctionAlways;
		}
	}

	MTLViewport convert(Viewport viewport)
	{
		return MTLViewport{
			.originX = viewport.originX,
			.originY = viewport.originY,
			.width = viewport.width,
			.height = viewport.height,
			.znear = viewport.zNear,
			.zfar = viewport.zFar
		};
	}

	MTLPixelFormat convert(PixelFormat pixelFormat)
	{
		switch (pixelFormat)
		{
			case PixelFormat::Undefined:				return MTLPixelFormatInvalid;
			case PixelFormat::A8Unorm:					return MTLPixelFormatA8Unorm;
			case PixelFormat::R8Unorm:					return MTLPixelFormatR8Unorm;
			case PixelFormat::R8Unorm_sRGB:				return MTLPixelFormatR8Unorm_sRGB;
			case PixelFormat::R8Snorm:					return MTLPixelFormatR8Snorm;
			case PixelFormat::R8Uint:					return MTLPixelFormatR8Uint;
			case PixelFormat::R8Sint:					return MTLPixelFormatR8Sint;
			case PixelFormat::R16Unorm:					return MTLPixelFormatR16Unorm;
			case PixelFormat::R16Snorm:					return MTLPixelFormatR16Snorm;
			case PixelFormat::R16Uint:					return MTLPixelFormatR16Uint;
			case PixelFormat::R16Sint:					return MTLPixelFormatR16Sint;
			case PixelFormat::R16Float:					return MTLPixelFormatR16Float;
			case PixelFormat::RG8Unorm:					return MTLPixelFormatRG8Unorm;
			case PixelFormat::RG8Unorm_sRGB:			return MTLPixelFormatRG8Unorm_sRGB;
			case PixelFormat::RG8Snorm:					return MTLPixelFormatRG8Snorm;
			case PixelFormat::RG8Uint:					return MTLPixelFormatRG8Uint;
			case PixelFormat::RG8Sint:					return MTLPixelFormatRG8Sint;
			case PixelFormat::B5G6R5Unorm:				return MTLPixelFormatB5G6R5Unorm;
			case PixelFormat::A1BGR5Unorm:				return MTLPixelFormatA1BGR5Unorm;
			case PixelFormat::ABGR4Unorm:				return MTLPixelFormatABGR4Unorm;
			case PixelFormat::BGR5A1Unorm:				return MTLPixelFormatBGR5A1Unorm;
			case PixelFormat::R32Uint:					return MTLPixelFormatR32Uint;
			case PixelFormat::R32Sint:					return MTLPixelFormatR32Sint;
			case PixelFormat::R32Float:					return MTLPixelFormatR32Float;
			case PixelFormat::RG16Unorm:				return MTLPixelFormatRG16Unorm;
			case PixelFormat::RG16Snorm:				return MTLPixelFormatRG16Snorm;
			case PixelFormat::RG16Uint:					return MTLPixelFormatRG16Uint;
			case PixelFormat::RG16Sint:					return MTLPixelFormatRG16Sint;
			case PixelFormat::RG16Float:				return MTLPixelFormatRG16Float;
			case PixelFormat::RGBA8Unorm:				return MTLPixelFormatRGBA8Unorm;
			case PixelFormat::RGBA8Unorm_sRGB:			return MTLPixelFormatRGBA8Unorm_sRGB;
			case PixelFormat::RGBA8Snorm:				return MTLPixelFormatRGBA8Snorm;
			case PixelFormat::RGBA8Uint:				return MTLPixelFormatRGBA8Uint;
			case PixelFormat::RGBA8Sint:				return MTLPixelFormatRGBA8Sint;
			case PixelFormat::BGRA8Unorm:				return MTLPixelFormatBGRA8Unorm;
			case PixelFormat::BGRA8Unorm_sRGB:			return MTLPixelFormatBGRA8Unorm_sRGB;
			case PixelFormat::RGB10A2Unorm:				return MTLPixelFormatRGB10A2Unorm;
			case PixelFormat::RGB10A2Uint:				return MTLPixelFormatRGB10A2Uint;
			case PixelFormat::RG11B10Float:				return MTLPixelFormatRG11B10Float;
			case PixelFormat::RGB9E5Float:				return MTLPixelFormatRGB9E5Float;
			case PixelFormat::BGR10A2Unorm:				return MTLPixelFormatBGR10A2Unorm;
			case PixelFormat::RG32Uint:					return MTLPixelFormatRG32Uint;
			case PixelFormat::RG32Sint:					return MTLPixelFormatRG32Sint;
			case PixelFormat::RG32Float:				return MTLPixelFormatRG32Float;
			case PixelFormat::RGBA16Unorm:				return MTLPixelFormatRGBA16Unorm;
			case PixelFormat::RGBA16Snorm:				return MTLPixelFormatRGBA16Snorm;
			case PixelFormat::RGBA16Uint:				return MTLPixelFormatRGBA16Uint;
			case PixelFormat::RGBA16Sint:				return MTLPixelFormatRGBA16Sint;
			case PixelFormat::RGBA16Float:				return MTLPixelFormatRGBA16Float;
			case PixelFormat::RGBA32Uint:				return MTLPixelFormatRGBA32Uint;
			case PixelFormat::RGBA32Sint:				return MTLPixelFormatRGBA32Sint;
			case PixelFormat::RGBA32Float:				return MTLPixelFormatRGBA32Float;
			case PixelFormat::BC1_RGBA:					return MTLPixelFormatBC1_RGBA;
			case PixelFormat::BC1_RGBA_sRGB:			return MTLPixelFormatBC1_RGBA_sRGB;
			case PixelFormat::BC2_RGBA:					return MTLPixelFormatBC2_RGBA;
			case PixelFormat::BC2_RGBA_sRGB:			return MTLPixelFormatBC2_RGBA_sRGB;
			case PixelFormat::BC3_RGBA:					return MTLPixelFormatBC3_RGBA;
			case PixelFormat::BC3_RGBA_sRGB:			return MTLPixelFormatBC3_RGBA_sRGB;
			case PixelFormat::BC4_RUnorm:				return MTLPixelFormatBC4_RUnorm;
			case PixelFormat::BC4_RSnorm:				return MTLPixelFormatBC4_RSnorm;
			case PixelFormat::BC5_RGUnorm:				return MTLPixelFormatBC5_RGUnorm;
			case PixelFormat::BC5_RGSnorm:				return MTLPixelFormatBC5_RGSnorm;
			case PixelFormat::BC6H_RGBFloat:			return MTLPixelFormatBC6H_RGBFloat;
			case PixelFormat::BC6H_RGBUfloat:			return MTLPixelFormatBC6H_RGBUfloat;
			case PixelFormat::BC7_RGBAUnorm:			return MTLPixelFormatBC7_RGBAUnorm;
			case PixelFormat::BC7_RGBAUnorm_sRGB:		return MTLPixelFormatBC7_RGBAUnorm_sRGB;
			case PixelFormat::PVRTC_RGB_2BPP:			return MTLPixelFormatPVRTC_RGB_2BPP;
			case PixelFormat::PVRTC_RGB_2BPP_sRGB:		return MTLPixelFormatPVRTC_RGB_2BPP_sRGB;
			case PixelFormat::PVRTC_RGB_4BPP:			return MTLPixelFormatPVRTC_RGB_4BPP;
			case PixelFormat::PVRTC_RGB_4BPP_sRGB:		return MTLPixelFormatPVRTC_RGB_4BPP_sRGB;
			case PixelFormat::PVRTC_RGBA_2BPP:			return MTLPixelFormatPVRTC_RGBA_2BPP;
			case PixelFormat::PVRTC_RGBA_2BPP_sRGB:		return MTLPixelFormatPVRTC_RGBA_2BPP_sRGB;
			case PixelFormat::PVRTC_RGBA_4BPP:			return MTLPixelFormatPVRTC_RGBA_4BPP;
			case PixelFormat::PVRTC_RGBA_4BPP_sRGB:		return MTLPixelFormatPVRTC_RGBA_4BPP_sRGB;
			case PixelFormat::EAC_R11Unorm:				return MTLPixelFormatEAC_R11Unorm;
			case PixelFormat::EAC_R11Snorm:				return MTLPixelFormatEAC_R11Snorm;
			case PixelFormat::EAC_RG11Unorm:			return MTLPixelFormatEAC_RG11Unorm;
			case PixelFormat::EAC_RG11Snorm:			return MTLPixelFormatEAC_RG11Snorm;
			case PixelFormat::EAC_RGBA8:				return MTLPixelFormatEAC_RGBA8;
			case PixelFormat::EAC_RGBA8_sRGB:			return MTLPixelFormatEAC_RGBA8_sRGB;
			case PixelFormat::ETC2_RGB8:				return MTLPixelFormatETC2_RGB8;
			case PixelFormat::ETC2_RGB8_sRGB:			return MTLPixelFormatETC2_RGB8_sRGB;
			case PixelFormat::ETC2_RGB8A1:				return MTLPixelFormatETC2_RGB8A1;
			case PixelFormat::ETC2_RGB8A1_sRGB:			return MTLPixelFormatETC2_RGB8A1_sRGB;
			case PixelFormat::ASTC_4x4_sRGB:			return MTLPixelFormatASTC_4x4_sRGB;
			case PixelFormat::ASTC_5x4_sRGB:			return MTLPixelFormatASTC_5x4_sRGB;
			case PixelFormat::ASTC_5x5_sRGB:			return MTLPixelFormatASTC_5x5_sRGB;
			case PixelFormat::ASTC_6x5_sRGB:			return MTLPixelFormatASTC_6x5_sRGB;
			case PixelFormat::ASTC_6x6_sRGB:			return MTLPixelFormatASTC_6x6_sRGB;
			case PixelFormat::ASTC_8x5_sRGB:			return MTLPixelFormatASTC_8x5_sRGB;
			case PixelFormat::ASTC_8x6_sRGB:			return MTLPixelFormatASTC_8x6_sRGB;
			case PixelFormat::ASTC_8x8_sRGB:			return MTLPixelFormatASTC_8x8_sRGB;
			case PixelFormat::ASTC_10x5_sRGB:			return MTLPixelFormatASTC_10x5_sRGB;
			case PixelFormat::ASTC_10x6_sRGB:			return MTLPixelFormatASTC_10x6_sRGB;
			case PixelFormat::ASTC_10x8_sRGB:			return MTLPixelFormatASTC_10x8_sRGB;
			case PixelFormat::ASTC_10x10_sRGB:			return MTLPixelFormatASTC_10x10_sRGB;
			case PixelFormat::ASTC_12x10_sRGB:			return MTLPixelFormatASTC_12x10_sRGB;
			case PixelFormat::ASTC_12x12_sRGB:			return MTLPixelFormatASTC_12x12_sRGB;
			case PixelFormat::ASTC_4x4_LDR:				return MTLPixelFormatASTC_4x4_LDR;
			case PixelFormat::ASTC_5x4_LDR:				return MTLPixelFormatASTC_5x4_LDR;
			case PixelFormat::ASTC_5x5_LDR:				return MTLPixelFormatASTC_5x5_LDR;
			case PixelFormat::ASTC_6x5_LDR:				return MTLPixelFormatASTC_6x5_LDR;
			case PixelFormat::ASTC_6x6_LDR:				return MTLPixelFormatASTC_6x6_LDR;
			case PixelFormat::ASTC_8x5_LDR:				return MTLPixelFormatASTC_8x5_LDR;
			case PixelFormat::ASTC_8x6_LDR:				return MTLPixelFormatASTC_8x6_LDR;
			case PixelFormat::ASTC_8x8_LDR:				return MTLPixelFormatASTC_8x8_LDR;
			case PixelFormat::ASTC_10x5_LDR:			return MTLPixelFormatASTC_10x5_LDR;
			case PixelFormat::ASTC_10x6_LDR:			return MTLPixelFormatASTC_10x6_LDR;
			case PixelFormat::ASTC_10x8_LDR:			return MTLPixelFormatASTC_10x8_LDR;
			case PixelFormat::ASTC_10x10_LDR:			return MTLPixelFormatASTC_10x10_LDR;
			case PixelFormat::ASTC_12x10_LDR:			return MTLPixelFormatASTC_12x10_LDR;
			case PixelFormat::ASTC_12x12_LDR:			return MTLPixelFormatASTC_12x12_LDR;
			case PixelFormat::ASTC_4x4_HDR:				return MTLPixelFormatASTC_4x4_HDR;
			case PixelFormat::ASTC_5x4_HDR:				return MTLPixelFormatASTC_5x4_HDR;
			case PixelFormat::ASTC_5x5_HDR:				return MTLPixelFormatASTC_5x5_HDR;
			case PixelFormat::ASTC_6x5_HDR:				return MTLPixelFormatASTC_6x5_HDR;
			case PixelFormat::ASTC_6x6_HDR:				return MTLPixelFormatASTC_6x6_HDR;
			case PixelFormat::ASTC_8x5_HDR:				return MTLPixelFormatASTC_8x5_HDR;
			case PixelFormat::ASTC_8x6_HDR:				return MTLPixelFormatASTC_8x6_HDR;
			case PixelFormat::ASTC_8x8_HDR:				return MTLPixelFormatASTC_8x8_HDR;
			case PixelFormat::ASTC_10x5_HDR:			return MTLPixelFormatASTC_10x5_HDR;
			case PixelFormat::ASTC_10x6_HDR:			return MTLPixelFormatASTC_10x6_HDR;
			case PixelFormat::ASTC_10x8_HDR:			return MTLPixelFormatASTC_10x8_HDR;
			case PixelFormat::ASTC_10x10_HDR:			return MTLPixelFormatASTC_10x10_HDR;
			case PixelFormat::ASTC_12x10_HDR:			return MTLPixelFormatASTC_12x10_HDR;
			case PixelFormat::ASTC_12x12_HDR:			return MTLPixelFormatASTC_12x12_HDR;
			case PixelFormat::GBGR422:					return MTLPixelFormatGBGR422;
			case PixelFormat::BGRG422:					return MTLPixelFormatBGRG422;
			case PixelFormat::Depth16Unorm:				return MTLPixelFormatDepth16Unorm;
			case PixelFormat::Depth32Float:				return MTLPixelFormatDepth32Float;
			case PixelFormat::Stencil8:					return MTLPixelFormatStencil8;
			case PixelFormat::Depth24Unorm_Stencil8:	return MTLPixelFormatDepth24Unorm_Stencil8;
			case PixelFormat::Depth32Float_Stencil8:	return MTLPixelFormatDepth32Float_Stencil8;
			case PixelFormat::X32_Stencil8:				return MTLPixelFormatX32_Stencil8;
			case PixelFormat::X24_Stencil8:				return MTLPixelFormatX24_Stencil8;
			case PixelFormat::BGRA10_XR:				return MTLPixelFormatBGRA10_XR;
			case PixelFormat::BGRA10_XR_sRGB:			return MTLPixelFormatBGRA10_XR_sRGB;
			case PixelFormat::BGR10_XR:					return MTLPixelFormatBGR10_XR;
			case PixelFormat::BGR10_XR_sRGB:			return MTLPixelFormatBGR10_XR_sRGB;
		}
	}
}