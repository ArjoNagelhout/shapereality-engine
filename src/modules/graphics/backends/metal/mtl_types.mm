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

    MTLCullMode convert(CullMode cullMode)
    {
        switch (cullMode)
        {
            case CullMode::None: return MTLCullModeNone;
            case CullMode::Front: return MTLCullModeFront;
            case CullMode::Back: return MTLCullModeBack;
        }
    }

    MTLTriangleFillMode convert(TriangleFillMode triangleFillMode)
    {
        switch (triangleFillMode)
        {
            case TriangleFillMode::Fill: return MTLTriangleFillModeFill;
            case TriangleFillMode::Lines: return MTLTriangleFillModeLines;
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

    NSRange convert(Range range)
    {
        return NSRange{
            .location = range.offset,
            .length = range.length
        };
    }

    MTLScissorRect convert(ScissorRect scissorRect)
    {
        return MTLScissorRect{
            .x = scissorRect.x,
            .y = scissorRect.y,
            .width = scissorRect.width,
            .height = scissorRect.height
        };
    }

    MTLPixelFormat convert(PixelFormat pixelFormat)
    {
        switch (pixelFormat)
        {
            case PixelFormat::Undefined: return MTLPixelFormatInvalid;
            case PixelFormat::A8Unorm: return MTLPixelFormatA8Unorm;
            case PixelFormat::R8Unorm: return MTLPixelFormatR8Unorm;
            case PixelFormat::R8Unorm_sRGB: return MTLPixelFormatR8Unorm_sRGB;
            case PixelFormat::R8Snorm: return MTLPixelFormatR8Snorm;
            case PixelFormat::R8Uint: return MTLPixelFormatR8Uint;
            case PixelFormat::R8Sint: return MTLPixelFormatR8Sint;
            case PixelFormat::R16Unorm: return MTLPixelFormatR16Unorm;
            case PixelFormat::R16Snorm: return MTLPixelFormatR16Snorm;
            case PixelFormat::R16Uint: return MTLPixelFormatR16Uint;
            case PixelFormat::R16Sint: return MTLPixelFormatR16Sint;
            case PixelFormat::R16Float: return MTLPixelFormatR16Float;
            case PixelFormat::RG8Unorm: return MTLPixelFormatRG8Unorm;
            case PixelFormat::RG8Unorm_sRGB: return MTLPixelFormatRG8Unorm_sRGB;
            case PixelFormat::RG8Snorm: return MTLPixelFormatRG8Snorm;
            case PixelFormat::RG8Uint: return MTLPixelFormatRG8Uint;
            case PixelFormat::RG8Sint: return MTLPixelFormatRG8Sint;
            case PixelFormat::B5G6R5Unorm: return MTLPixelFormatB5G6R5Unorm;
            case PixelFormat::A1BGR5Unorm: return MTLPixelFormatA1BGR5Unorm;
            case PixelFormat::ABGR4Unorm: return MTLPixelFormatABGR4Unorm;
            case PixelFormat::BGR5A1Unorm: return MTLPixelFormatBGR5A1Unorm;
            case PixelFormat::R32Uint: return MTLPixelFormatR32Uint;
            case PixelFormat::R32Sint: return MTLPixelFormatR32Sint;
            case PixelFormat::R32Float: return MTLPixelFormatR32Float;
            case PixelFormat::RG16Unorm: return MTLPixelFormatRG16Unorm;
            case PixelFormat::RG16Snorm: return MTLPixelFormatRG16Snorm;
            case PixelFormat::RG16Uint: return MTLPixelFormatRG16Uint;
            case PixelFormat::RG16Sint: return MTLPixelFormatRG16Sint;
            case PixelFormat::RG16Float: return MTLPixelFormatRG16Float;
            case PixelFormat::RGBA8Unorm: return MTLPixelFormatRGBA8Unorm;
            case PixelFormat::RGBA8Unorm_sRGB: return MTLPixelFormatRGBA8Unorm_sRGB;
            case PixelFormat::RGBA8Snorm: return MTLPixelFormatRGBA8Snorm;
            case PixelFormat::RGBA8Uint: return MTLPixelFormatRGBA8Uint;
            case PixelFormat::RGBA8Sint: return MTLPixelFormatRGBA8Sint;
            case PixelFormat::BGRA8Unorm: return MTLPixelFormatBGRA8Unorm;
            case PixelFormat::BGRA8Unorm_sRGB: return MTLPixelFormatBGRA8Unorm_sRGB;
            case PixelFormat::RGB10A2Unorm: return MTLPixelFormatRGB10A2Unorm;
            case PixelFormat::RGB10A2Uint: return MTLPixelFormatRGB10A2Uint;
            case PixelFormat::RG11B10Float: return MTLPixelFormatRG11B10Float;
            case PixelFormat::RGB9E5Float: return MTLPixelFormatRGB9E5Float;
            case PixelFormat::BGR10A2Unorm: return MTLPixelFormatBGR10A2Unorm;
            case PixelFormat::RG32Uint: return MTLPixelFormatRG32Uint;
            case PixelFormat::RG32Sint: return MTLPixelFormatRG32Sint;
            case PixelFormat::RG32Float: return MTLPixelFormatRG32Float;
            case PixelFormat::RGBA16Unorm: return MTLPixelFormatRGBA16Unorm;
            case PixelFormat::RGBA16Snorm: return MTLPixelFormatRGBA16Snorm;
            case PixelFormat::RGBA16Uint: return MTLPixelFormatRGBA16Uint;
            case PixelFormat::RGBA16Sint: return MTLPixelFormatRGBA16Sint;
            case PixelFormat::RGBA16Float: return MTLPixelFormatRGBA16Float;
            case PixelFormat::RGBA32Uint: return MTLPixelFormatRGBA32Uint;
            case PixelFormat::RGBA32Sint: return MTLPixelFormatRGBA32Sint;
            case PixelFormat::RGBA32Float: return MTLPixelFormatRGBA32Float;
            case PixelFormat::BC1_RGBA: return MTLPixelFormatBC1_RGBA;
            case PixelFormat::BC1_RGBA_sRGB: return MTLPixelFormatBC1_RGBA_sRGB;
            case PixelFormat::BC2_RGBA: return MTLPixelFormatBC2_RGBA;
            case PixelFormat::BC2_RGBA_sRGB: return MTLPixelFormatBC2_RGBA_sRGB;
            case PixelFormat::BC3_RGBA: return MTLPixelFormatBC3_RGBA;
            case PixelFormat::BC3_RGBA_sRGB: return MTLPixelFormatBC3_RGBA_sRGB;
            case PixelFormat::BC4_RUnorm: return MTLPixelFormatBC4_RUnorm;
            case PixelFormat::BC4_RSnorm: return MTLPixelFormatBC4_RSnorm;
            case PixelFormat::BC5_RGUnorm: return MTLPixelFormatBC5_RGUnorm;
            case PixelFormat::BC5_RGSnorm: return MTLPixelFormatBC5_RGSnorm;
            case PixelFormat::BC6H_RGBFloat: return MTLPixelFormatBC6H_RGBFloat;
            case PixelFormat::BC6H_RGBUfloat: return MTLPixelFormatBC6H_RGBUfloat;
            case PixelFormat::BC7_RGBAUnorm: return MTLPixelFormatBC7_RGBAUnorm;
            case PixelFormat::BC7_RGBAUnorm_sRGB: return MTLPixelFormatBC7_RGBAUnorm_sRGB;
            case PixelFormat::PVRTC_RGB_2BPP: return MTLPixelFormatPVRTC_RGB_2BPP;
            case PixelFormat::PVRTC_RGB_2BPP_sRGB: return MTLPixelFormatPVRTC_RGB_2BPP_sRGB;
            case PixelFormat::PVRTC_RGB_4BPP: return MTLPixelFormatPVRTC_RGB_4BPP;
            case PixelFormat::PVRTC_RGB_4BPP_sRGB: return MTLPixelFormatPVRTC_RGB_4BPP_sRGB;
            case PixelFormat::PVRTC_RGBA_2BPP: return MTLPixelFormatPVRTC_RGBA_2BPP;
            case PixelFormat::PVRTC_RGBA_2BPP_sRGB: return MTLPixelFormatPVRTC_RGBA_2BPP_sRGB;
            case PixelFormat::PVRTC_RGBA_4BPP: return MTLPixelFormatPVRTC_RGBA_4BPP;
            case PixelFormat::PVRTC_RGBA_4BPP_sRGB: return MTLPixelFormatPVRTC_RGBA_4BPP_sRGB;
            case PixelFormat::EAC_R11Unorm: return MTLPixelFormatEAC_R11Unorm;
            case PixelFormat::EAC_R11Snorm: return MTLPixelFormatEAC_R11Snorm;
            case PixelFormat::EAC_RG11Unorm: return MTLPixelFormatEAC_RG11Unorm;
            case PixelFormat::EAC_RG11Snorm: return MTLPixelFormatEAC_RG11Snorm;
            case PixelFormat::EAC_RGBA8: return MTLPixelFormatEAC_RGBA8;
            case PixelFormat::EAC_RGBA8_sRGB: return MTLPixelFormatEAC_RGBA8_sRGB;
            case PixelFormat::ETC2_RGB8: return MTLPixelFormatETC2_RGB8;
            case PixelFormat::ETC2_RGB8_sRGB: return MTLPixelFormatETC2_RGB8_sRGB;
            case PixelFormat::ETC2_RGB8A1: return MTLPixelFormatETC2_RGB8A1;
            case PixelFormat::ETC2_RGB8A1_sRGB: return MTLPixelFormatETC2_RGB8A1_sRGB;
            case PixelFormat::ASTC_4x4_sRGB: return MTLPixelFormatASTC_4x4_sRGB;
            case PixelFormat::ASTC_5x4_sRGB: return MTLPixelFormatASTC_5x4_sRGB;
            case PixelFormat::ASTC_5x5_sRGB: return MTLPixelFormatASTC_5x5_sRGB;
            case PixelFormat::ASTC_6x5_sRGB: return MTLPixelFormatASTC_6x5_sRGB;
            case PixelFormat::ASTC_6x6_sRGB: return MTLPixelFormatASTC_6x6_sRGB;
            case PixelFormat::ASTC_8x5_sRGB: return MTLPixelFormatASTC_8x5_sRGB;
            case PixelFormat::ASTC_8x6_sRGB: return MTLPixelFormatASTC_8x6_sRGB;
            case PixelFormat::ASTC_8x8_sRGB: return MTLPixelFormatASTC_8x8_sRGB;
            case PixelFormat::ASTC_10x5_sRGB: return MTLPixelFormatASTC_10x5_sRGB;
            case PixelFormat::ASTC_10x6_sRGB: return MTLPixelFormatASTC_10x6_sRGB;
            case PixelFormat::ASTC_10x8_sRGB: return MTLPixelFormatASTC_10x8_sRGB;
            case PixelFormat::ASTC_10x10_sRGB: return MTLPixelFormatASTC_10x10_sRGB;
            case PixelFormat::ASTC_12x10_sRGB: return MTLPixelFormatASTC_12x10_sRGB;
            case PixelFormat::ASTC_12x12_sRGB: return MTLPixelFormatASTC_12x12_sRGB;
            case PixelFormat::ASTC_4x4_LDR: return MTLPixelFormatASTC_4x4_LDR;
            case PixelFormat::ASTC_5x4_LDR: return MTLPixelFormatASTC_5x4_LDR;
            case PixelFormat::ASTC_5x5_LDR: return MTLPixelFormatASTC_5x5_LDR;
            case PixelFormat::ASTC_6x5_LDR: return MTLPixelFormatASTC_6x5_LDR;
            case PixelFormat::ASTC_6x6_LDR: return MTLPixelFormatASTC_6x6_LDR;
            case PixelFormat::ASTC_8x5_LDR: return MTLPixelFormatASTC_8x5_LDR;
            case PixelFormat::ASTC_8x6_LDR: return MTLPixelFormatASTC_8x6_LDR;
            case PixelFormat::ASTC_8x8_LDR: return MTLPixelFormatASTC_8x8_LDR;
            case PixelFormat::ASTC_10x5_LDR: return MTLPixelFormatASTC_10x5_LDR;
            case PixelFormat::ASTC_10x6_LDR: return MTLPixelFormatASTC_10x6_LDR;
            case PixelFormat::ASTC_10x8_LDR: return MTLPixelFormatASTC_10x8_LDR;
            case PixelFormat::ASTC_10x10_LDR: return MTLPixelFormatASTC_10x10_LDR;
            case PixelFormat::ASTC_12x10_LDR: return MTLPixelFormatASTC_12x10_LDR;
            case PixelFormat::ASTC_12x12_LDR: return MTLPixelFormatASTC_12x12_LDR;
            case PixelFormat::ASTC_4x4_HDR: return MTLPixelFormatASTC_4x4_HDR;
            case PixelFormat::ASTC_5x4_HDR: return MTLPixelFormatASTC_5x4_HDR;
            case PixelFormat::ASTC_5x5_HDR: return MTLPixelFormatASTC_5x5_HDR;
            case PixelFormat::ASTC_6x5_HDR: return MTLPixelFormatASTC_6x5_HDR;
            case PixelFormat::ASTC_6x6_HDR: return MTLPixelFormatASTC_6x6_HDR;
            case PixelFormat::ASTC_8x5_HDR: return MTLPixelFormatASTC_8x5_HDR;
            case PixelFormat::ASTC_8x6_HDR: return MTLPixelFormatASTC_8x6_HDR;
            case PixelFormat::ASTC_8x8_HDR: return MTLPixelFormatASTC_8x8_HDR;
            case PixelFormat::ASTC_10x5_HDR: return MTLPixelFormatASTC_10x5_HDR;
            case PixelFormat::ASTC_10x6_HDR: return MTLPixelFormatASTC_10x6_HDR;
            case PixelFormat::ASTC_10x8_HDR: return MTLPixelFormatASTC_10x8_HDR;
            case PixelFormat::ASTC_10x10_HDR: return MTLPixelFormatASTC_10x10_HDR;
            case PixelFormat::ASTC_12x10_HDR: return MTLPixelFormatASTC_12x10_HDR;
            case PixelFormat::ASTC_12x12_HDR: return MTLPixelFormatASTC_12x12_HDR;
            case PixelFormat::GBGR422: return MTLPixelFormatGBGR422;
            case PixelFormat::BGRG422: return MTLPixelFormatBGRG422;
            case PixelFormat::Depth16Unorm: return MTLPixelFormatDepth16Unorm;
            case PixelFormat::Depth32Float: return MTLPixelFormatDepth32Float;
            case PixelFormat::Stencil8: return MTLPixelFormatStencil8;
            case PixelFormat::Depth24Unorm_Stencil8: return MTLPixelFormatDepth24Unorm_Stencil8;
            case PixelFormat::Depth32Float_Stencil8: return MTLPixelFormatDepth32Float_Stencil8;
            case PixelFormat::X32_Stencil8: return MTLPixelFormatX32_Stencil8;
            case PixelFormat::X24_Stencil8: return MTLPixelFormatX24_Stencil8;
            case PixelFormat::BGRA10_XR: return MTLPixelFormatBGRA10_XR;
            case PixelFormat::BGRA10_XR_sRGB: return MTLPixelFormatBGRA10_XR_sRGB;
            case PixelFormat::BGR10_XR: return MTLPixelFormatBGR10_XR;
            case PixelFormat::BGR10_XR_sRGB: return MTLPixelFormatBGR10_XR_sRGB;
        }
    }

    PixelFormat convert(MTLPixelFormat metalPixelFormat)
    {
        switch (metalPixelFormat)
        {
            case MTLPixelFormatInvalid: return PixelFormat::Undefined;
            case MTLPixelFormatA8Unorm: return PixelFormat::A8Unorm;
            case MTLPixelFormatR8Unorm: return PixelFormat::R8Unorm;
            case MTLPixelFormatR8Unorm_sRGB: return PixelFormat::R8Unorm_sRGB;
            case MTLPixelFormatR8Snorm: return PixelFormat::R8Snorm;
            case MTLPixelFormatR8Uint: return PixelFormat::R8Uint;
            case MTLPixelFormatR8Sint: return PixelFormat::R8Sint;
            case MTLPixelFormatR16Unorm: return PixelFormat::R16Unorm;
            case MTLPixelFormatR16Snorm: return PixelFormat::R16Snorm;
            case MTLPixelFormatR16Uint: return PixelFormat::R16Uint;
            case MTLPixelFormatR16Sint: return PixelFormat::R16Sint;
            case MTLPixelFormatR16Float: return PixelFormat::R16Float;
            case MTLPixelFormatRG8Unorm: return PixelFormat::RG8Unorm;
            case MTLPixelFormatRG8Unorm_sRGB: return PixelFormat::RG8Unorm_sRGB;
            case MTLPixelFormatRG8Snorm: return PixelFormat::RG8Snorm;
            case MTLPixelFormatRG8Uint: return PixelFormat::RG8Uint;
            case MTLPixelFormatRG8Sint: return PixelFormat::RG8Sint;
            case MTLPixelFormatB5G6R5Unorm: return PixelFormat::B5G6R5Unorm;
            case MTLPixelFormatA1BGR5Unorm: return PixelFormat::A1BGR5Unorm;
            case MTLPixelFormatABGR4Unorm: return PixelFormat::ABGR4Unorm;
            case MTLPixelFormatBGR5A1Unorm: return PixelFormat::BGR5A1Unorm;
            case MTLPixelFormatR32Uint: return PixelFormat::R32Uint;
            case MTLPixelFormatR32Sint: return PixelFormat::R32Sint;
            case MTLPixelFormatR32Float: return PixelFormat::R32Float;
            case MTLPixelFormatRG16Unorm: return PixelFormat::RG16Unorm;
            case MTLPixelFormatRG16Snorm: return PixelFormat::RG16Snorm;
            case MTLPixelFormatRG16Uint: return PixelFormat::RG16Uint;
            case MTLPixelFormatRG16Sint: return PixelFormat::RG16Sint;
            case MTLPixelFormatRG16Float: return PixelFormat::RG16Float;
            case MTLPixelFormatRGBA8Unorm: return PixelFormat::RGBA8Unorm;
            case MTLPixelFormatRGBA8Unorm_sRGB: return PixelFormat::RGBA8Unorm_sRGB;
            case MTLPixelFormatRGBA8Snorm: return PixelFormat::RGBA8Snorm;
            case MTLPixelFormatRGBA8Uint: return PixelFormat::RGBA8Uint;
            case MTLPixelFormatRGBA8Sint: return PixelFormat::RGBA8Sint;
            case MTLPixelFormatBGRA8Unorm: return PixelFormat::BGRA8Unorm;
            case MTLPixelFormatBGRA8Unorm_sRGB: return PixelFormat::BGRA8Unorm_sRGB;
            case MTLPixelFormatRGB10A2Unorm: return PixelFormat::RGB10A2Unorm;
            case MTLPixelFormatRGB10A2Uint: return PixelFormat::RGB10A2Uint;
            case MTLPixelFormatRG11B10Float: return PixelFormat::RG11B10Float;
            case MTLPixelFormatRGB9E5Float: return PixelFormat::RGB9E5Float;
            case MTLPixelFormatBGR10A2Unorm: return PixelFormat::BGR10A2Unorm;
            case MTLPixelFormatRG32Uint: return PixelFormat::RG32Uint;
            case MTLPixelFormatRG32Sint: return PixelFormat::RG32Sint;
            case MTLPixelFormatRG32Float: return PixelFormat::RG32Float;
            case MTLPixelFormatRGBA16Unorm: return PixelFormat::RGBA16Unorm;
            case MTLPixelFormatRGBA16Snorm: return PixelFormat::RGBA16Snorm;
            case MTLPixelFormatRGBA16Uint: return PixelFormat::RGBA16Uint;
            case MTLPixelFormatRGBA16Sint: return PixelFormat::RGBA16Sint;
            case MTLPixelFormatRGBA16Float: return PixelFormat::RGBA16Float;
            case MTLPixelFormatRGBA32Uint: return PixelFormat::RGBA32Uint;
            case MTLPixelFormatRGBA32Sint: return PixelFormat::RGBA32Sint;
            case MTLPixelFormatRGBA32Float: return PixelFormat::RGBA32Float;
            case MTLPixelFormatBC1_RGBA: return PixelFormat::BC1_RGBA;
            case MTLPixelFormatBC1_RGBA_sRGB: return PixelFormat::BC1_RGBA_sRGB;
            case MTLPixelFormatBC2_RGBA: return PixelFormat::BC2_RGBA;
            case MTLPixelFormatBC2_RGBA_sRGB: return PixelFormat::BC2_RGBA_sRGB;
            case MTLPixelFormatBC3_RGBA: return PixelFormat::BC3_RGBA;
            case MTLPixelFormatBC3_RGBA_sRGB: return PixelFormat::BC3_RGBA_sRGB;
            case MTLPixelFormatBC4_RUnorm: return PixelFormat::BC4_RUnorm;
            case MTLPixelFormatBC4_RSnorm: return PixelFormat::BC4_RSnorm;
            case MTLPixelFormatBC5_RGUnorm: return PixelFormat::BC5_RGUnorm;
            case MTLPixelFormatBC5_RGSnorm: return PixelFormat::BC5_RGSnorm;
            case MTLPixelFormatBC6H_RGBFloat: return PixelFormat::BC6H_RGBFloat;
            case MTLPixelFormatBC6H_RGBUfloat: return PixelFormat::BC6H_RGBUfloat;
            case MTLPixelFormatBC7_RGBAUnorm: return PixelFormat::BC7_RGBAUnorm;
            case MTLPixelFormatBC7_RGBAUnorm_sRGB: return PixelFormat::BC7_RGBAUnorm_sRGB;
            case MTLPixelFormatPVRTC_RGB_2BPP: return PixelFormat::PVRTC_RGB_2BPP;
            case MTLPixelFormatPVRTC_RGB_2BPP_sRGB: return PixelFormat::PVRTC_RGB_2BPP_sRGB;
            case MTLPixelFormatPVRTC_RGB_4BPP: return PixelFormat::PVRTC_RGB_4BPP;
            case MTLPixelFormatPVRTC_RGB_4BPP_sRGB: return PixelFormat::PVRTC_RGB_4BPP_sRGB;
            case MTLPixelFormatPVRTC_RGBA_2BPP: return PixelFormat::PVRTC_RGBA_2BPP;
            case MTLPixelFormatPVRTC_RGBA_2BPP_sRGB: return PixelFormat::PVRTC_RGBA_2BPP_sRGB;
            case MTLPixelFormatPVRTC_RGBA_4BPP: return PixelFormat::PVRTC_RGBA_4BPP;
            case MTLPixelFormatPVRTC_RGBA_4BPP_sRGB: return PixelFormat::PVRTC_RGBA_4BPP_sRGB;
            case MTLPixelFormatEAC_R11Unorm: return PixelFormat::EAC_R11Unorm;
            case MTLPixelFormatEAC_R11Snorm: return PixelFormat::EAC_R11Snorm;
            case MTLPixelFormatEAC_RG11Unorm: return PixelFormat::EAC_RG11Unorm;
            case MTLPixelFormatEAC_RG11Snorm: return PixelFormat::EAC_RG11Snorm;
            case MTLPixelFormatEAC_RGBA8: return PixelFormat::EAC_RGBA8;
            case MTLPixelFormatEAC_RGBA8_sRGB: return PixelFormat::EAC_RGBA8_sRGB;
            case MTLPixelFormatETC2_RGB8: return PixelFormat::ETC2_RGB8;
            case MTLPixelFormatETC2_RGB8_sRGB: return PixelFormat::ETC2_RGB8_sRGB;
            case MTLPixelFormatETC2_RGB8A1: return PixelFormat::ETC2_RGB8A1;
            case MTLPixelFormatETC2_RGB8A1_sRGB: return PixelFormat::ETC2_RGB8A1_sRGB;
            case MTLPixelFormatASTC_4x4_sRGB: return PixelFormat::ASTC_4x4_sRGB;
            case MTLPixelFormatASTC_5x4_sRGB: return PixelFormat::ASTC_5x4_sRGB;
            case MTLPixelFormatASTC_5x5_sRGB: return PixelFormat::ASTC_5x5_sRGB;
            case MTLPixelFormatASTC_6x5_sRGB: return PixelFormat::ASTC_6x5_sRGB;
            case MTLPixelFormatASTC_6x6_sRGB: return PixelFormat::ASTC_6x6_sRGB;
            case MTLPixelFormatASTC_8x5_sRGB: return PixelFormat::ASTC_8x5_sRGB;
            case MTLPixelFormatASTC_8x6_sRGB: return PixelFormat::ASTC_8x6_sRGB;
            case MTLPixelFormatASTC_8x8_sRGB: return PixelFormat::ASTC_8x8_sRGB;
            case MTLPixelFormatASTC_10x5_sRGB: return PixelFormat::ASTC_10x5_sRGB;
            case MTLPixelFormatASTC_10x6_sRGB: return PixelFormat::ASTC_10x6_sRGB;
            case MTLPixelFormatASTC_10x8_sRGB: return PixelFormat::ASTC_10x8_sRGB;
            case MTLPixelFormatASTC_10x10_sRGB: return PixelFormat::ASTC_10x10_sRGB;
            case MTLPixelFormatASTC_12x10_sRGB: return PixelFormat::ASTC_12x10_sRGB;
            case MTLPixelFormatASTC_12x12_sRGB: return PixelFormat::ASTC_12x12_sRGB;
            case MTLPixelFormatASTC_4x4_LDR: return PixelFormat::ASTC_4x4_LDR;
            case MTLPixelFormatASTC_5x4_LDR: return PixelFormat::ASTC_5x4_LDR;
            case MTLPixelFormatASTC_5x5_LDR: return PixelFormat::ASTC_5x5_LDR;
            case MTLPixelFormatASTC_6x5_LDR: return PixelFormat::ASTC_6x5_LDR;
            case MTLPixelFormatASTC_6x6_LDR: return PixelFormat::ASTC_6x6_LDR;
            case MTLPixelFormatASTC_8x5_LDR: return PixelFormat::ASTC_8x5_LDR;
            case MTLPixelFormatASTC_8x6_LDR: return PixelFormat::ASTC_8x6_LDR;
            case MTLPixelFormatASTC_8x8_LDR: return PixelFormat::ASTC_8x8_LDR;
            case MTLPixelFormatASTC_10x5_LDR: return PixelFormat::ASTC_10x5_LDR;
            case MTLPixelFormatASTC_10x6_LDR: return PixelFormat::ASTC_10x6_LDR;
            case MTLPixelFormatASTC_10x8_LDR: return PixelFormat::ASTC_10x8_LDR;
            case MTLPixelFormatASTC_10x10_LDR: return PixelFormat::ASTC_10x10_LDR;
            case MTLPixelFormatASTC_12x10_LDR: return PixelFormat::ASTC_12x10_LDR;
            case MTLPixelFormatASTC_12x12_LDR: return PixelFormat::ASTC_12x12_LDR;
            case MTLPixelFormatASTC_4x4_HDR: return PixelFormat::ASTC_4x4_HDR;
            case MTLPixelFormatASTC_5x4_HDR: return PixelFormat::ASTC_5x4_HDR;
            case MTLPixelFormatASTC_5x5_HDR: return PixelFormat::ASTC_5x5_HDR;
            case MTLPixelFormatASTC_6x5_HDR: return PixelFormat::ASTC_6x5_HDR;
            case MTLPixelFormatASTC_6x6_HDR: return PixelFormat::ASTC_6x6_HDR;
            case MTLPixelFormatASTC_8x5_HDR: return PixelFormat::ASTC_8x5_HDR;
            case MTLPixelFormatASTC_8x6_HDR: return PixelFormat::ASTC_8x6_HDR;
            case MTLPixelFormatASTC_8x8_HDR: return PixelFormat::ASTC_8x8_HDR;
            case MTLPixelFormatASTC_10x5_HDR: return PixelFormat::ASTC_10x5_HDR;
            case MTLPixelFormatASTC_10x6_HDR: return PixelFormat::ASTC_10x6_HDR;
            case MTLPixelFormatASTC_10x8_HDR: return PixelFormat::ASTC_10x8_HDR;
            case MTLPixelFormatASTC_10x10_HDR: return PixelFormat::ASTC_10x10_HDR;
            case MTLPixelFormatASTC_12x10_HDR: return PixelFormat::ASTC_12x10_HDR;
            case MTLPixelFormatASTC_12x12_HDR: return PixelFormat::ASTC_12x12_HDR;
            case MTLPixelFormatGBGR422: return PixelFormat::GBGR422;
            case MTLPixelFormatBGRG422: return PixelFormat::BGRG422;
            case MTLPixelFormatDepth16Unorm: return PixelFormat::Depth16Unorm;
            case MTLPixelFormatDepth32Float: return PixelFormat::Depth32Float;
            case MTLPixelFormatStencil8: return PixelFormat::Stencil8;
            case MTLPixelFormatDepth24Unorm_Stencil8: return PixelFormat::Depth24Unorm_Stencil8;
            case MTLPixelFormatDepth32Float_Stencil8: return PixelFormat::Depth32Float_Stencil8;
            case MTLPixelFormatX32_Stencil8: return PixelFormat::X32_Stencil8;
            case MTLPixelFormatX24_Stencil8: return PixelFormat::X24_Stencil8;
            case MTLPixelFormatBGRA10_XR: return PixelFormat::BGRA10_XR;
            case MTLPixelFormatBGRA10_XR_sRGB: return PixelFormat::BGRA10_XR_sRGB;
            case MTLPixelFormatBGR10_XR: return PixelFormat::BGR10_XR;
            case MTLPixelFormatBGR10_XR_sRGB: return PixelFormat::BGR10_XR_sRGB;
        }
    }

    MTLStoreAction convert(StoreAction storeAction)
    {
        switch (storeAction)
        {
            case StoreAction::Store: return MTLStoreActionStore;
            case StoreAction::DontCare: return MTLStoreActionDontCare;
            case StoreAction::MultisampleResolve: return MTLStoreActionMultisampleResolve;
        }
    }

    StoreAction convert(MTLStoreAction metalStoreAction)
    {
        switch (metalStoreAction)
        {
            case MTLStoreActionStore: return StoreAction::Store;
            case MTLStoreActionDontCare: return StoreAction::DontCare;
            case MTLStoreActionMultisampleResolve: return StoreAction::MultisampleResolve;
            default: return StoreAction::DontCare;
        }
    }

    // LoadAction

    MTLLoadAction convert(LoadAction loadAction)
    {
        switch (loadAction)
        {
            case LoadAction::DontCare: return MTLLoadActionDontCare;
            case LoadAction::Clear: return MTLLoadActionClear;
            case LoadAction::Load: return MTLLoadActionLoad;
        }
    }

    LoadAction convert(MTLLoadAction metalLoadAction)
    {
        switch (metalLoadAction)
        {
            case MTLLoadActionDontCare: return LoadAction::DontCare;
            case MTLLoadActionClear: return LoadAction::Clear;
            case MTLLoadActionLoad: return LoadAction::Load;
        }
    }

    MTLMultisampleDepthResolveFilter convert(MultisampleDepthResolveFilter multisampleDepthResolveFilter)
    {
        switch (multisampleDepthResolveFilter)
        {
            case MultisampleDepthResolveFilter::Sample0: return MTLMultisampleDepthResolveFilterSample0;
            case MultisampleDepthResolveFilter::Min: return MTLMultisampleDepthResolveFilterMin;
            case MultisampleDepthResolveFilter::Max: return MTLMultisampleDepthResolveFilterMax;
        }
    }

    MultisampleDepthResolveFilter convert(MTLMultisampleDepthResolveFilter metalMultisampleDepthResolveFilter)
    {
        switch (metalMultisampleDepthResolveFilter)
        {
            case MTLMultisampleDepthResolveFilterSample0: return MultisampleDepthResolveFilter::Sample0;
            case MTLMultisampleDepthResolveFilterMin: return MultisampleDepthResolveFilter::Min;
            case MTLMultisampleDepthResolveFilterMax: return MultisampleDepthResolveFilter::Max;
        }
    }

    MTLClearColor convert(Color const& color)
    {
        return MTLClearColorMake(color.r, color.g, color.b, color.a);
    }

    Color convert(MTLClearColor metalClearColor)
    {
        return Color{
            .r = static_cast<float>(metalClearColor.red),
            .g = static_cast<float>(metalClearColor.green),
            .b = static_cast<float>(metalClearColor.blue),
            .a = static_cast<float>(metalClearColor.alpha)
        };
    }

    MTLTextureUsage convert(TextureUsage_ textureUsage)
    {
        MTLTextureUsage result{};
        result |= (textureUsage & TextureUsage_Unknown) ? MTLTextureUsageUnknown : 0;
        result |= (textureUsage & TextureUsage_ShaderRead) ? MTLTextureUsageShaderRead : 0;
        result |= (textureUsage & TextureUsage_ShaderWrite) ? MTLTextureUsageShaderWrite : 0;
        result |= (textureUsage & TextureUsage_ShaderAtomic) ? MTLTextureUsageShaderAtomic : 0;
        result |= (textureUsage & TextureUsage_RenderTarget) ? MTLTextureUsageRenderTarget : 0;
        return result;
    }

    TextureUsage_ convertFromMetal(MTLTextureUsage metalTextureUsage)
    {
        int result{};
        result |= (metalTextureUsage & MTLTextureUsageUnknown) ? TextureUsage_Unknown : 0;
        result |= (metalTextureUsage & MTLTextureUsageShaderRead) ? TextureUsage_ShaderRead : 0;
        result |= (metalTextureUsage & MTLTextureUsageShaderWrite) ? TextureUsage_ShaderWrite : 0;
        result |= (metalTextureUsage & MTLTextureUsageShaderAtomic) ? TextureUsage_ShaderAtomic : 0;
        result |= (metalTextureUsage & MTLTextureUsageRenderTarget) ? TextureUsage_RenderTarget : 0;
        return static_cast<TextureUsage_>(result);
    }

    MTLTextureType convert(TextureType textureType)
    {
        switch (textureType)
        {
            case TextureType::Type1D: return MTLTextureType1D;
            case TextureType::Type1DArray: return MTLTextureType1DArray;
            case TextureType::Type2D: return MTLTextureType2D;
            case TextureType::Type2DArray: return MTLTextureType2DArray;
            case TextureType::Type2DMultisample: return MTLTextureType2DMultisample;
            case TextureType::TypeCube: return MTLTextureTypeCube;
            case TextureType::TypeCubeArray: return MTLTextureTypeCubeArray;
            case TextureType::Type3D: return MTLTextureType3D;
            case TextureType::Type2DMultisampleArray: return MTLTextureType2DMultisampleArray;
            case TextureType::TypeTextureBuffer: return MTLTextureTypeTextureBuffer;
        }
    }

    TextureType convert(MTLTextureType metalTextureType)
    {
        switch (metalTextureType)
        {
            case MTLTextureType1D: return TextureType::Type1D;
            case MTLTextureType1DArray: return TextureType::Type1DArray;
            case MTLTextureType2D: return TextureType::Type2D;
            case MTLTextureType2DArray: return TextureType::Type2DArray;
            case MTLTextureType2DMultisample: return TextureType::Type2DMultisample;
            case MTLTextureTypeCube: return TextureType::TypeCube;
            case MTLTextureTypeCubeArray: return TextureType::TypeCubeArray;
            case MTLTextureType3D: return TextureType::Type3D;
            case MTLTextureType2DMultisampleArray: return TextureType::Type2DMultisampleArray;
            case MTLTextureTypeTextureBuffer: return TextureType::TypeTextureBuffer;
        }
    }
}