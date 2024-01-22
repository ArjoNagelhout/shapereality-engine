//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef SHAPEREALITY_RENDER_PIPELINE_STATE_H
#define SHAPEREALITY_RENDER_PIPELINE_STATE_H

#include <string>

#include "graphics.h"

namespace graphics
{
    class IShaderFunction;

    enum class PrimitiveTopologyType
    {
        Unspecified,
        Point,
        Line,
        Triangle
    };

    enum class VertexFormat
    {
        Invalid,
        UChar2,
        UChar3,
        UChar4,
        Char2,
        Char3,
        Char4,
        UChar2Normalized,
        UChar3Normalized,
        UChar4Normalized,
        Char2Normalized,
        Char3Normalized,
        Char4Normalized,
        UShort2,
        UShort3,
        UShort4,
        Short2,
        Short3,
        Short4,
        UShort2Normalized,
        UShort3Normalized,
        UShort4Normalized,
        Short2Normalized,
        Short3Normalized,
        Short4Normalized,
        Half2,
        Half3,
        Half4,
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
        UInt,
        UInt2,
        UInt3,
        UInt4,
        Int1010102Normalized,
        UInt1010102Normalized,
        UChar4Normalized_BGRA,
        UChar,
        Char,
        UCharNormalized,
        CharNormalized,
        UShort,
        Short,
        UShortNormalized,
        ShortNormalized,
        Half,
        FloatRG11B10,
        FloatRGB9E5,
    };

    struct VertexAttributeDescriptor
    {
        VertexFormat format;
        unsigned int offset;
        unsigned int bufferIndex;
    };

    enum class VertexStepFunction
    {
        Constant,
        PerVertex,
        PerInstance,
        PerPatch,
        PerPatchControlPoint
    };

    struct VertexBufferLayoutDescriptor
    {
        VertexStepFunction stepFunction;
        unsigned int stepRate;
        unsigned int stride;
    };

    struct VertexDescriptor
    {
        std::vector<VertexAttributeDescriptor> attributes;
        std::vector<VertexBufferLayoutDescriptor> layouts;
    };

    enum class TessellationFactorFormat
    {
        Half
    };

    enum class TessellationControlPointIndexType
    {
        None,
        UInt16,
        UInt32
    };

    enum class TessellationPartitionMode
    {
        Pow2,
        Integer,
        FractionalOdd,
        FractionalEven
    };

    enum class TessellationFactorStepFunction
    {
        Constant
    };

    enum class Mutability
    {
        Default,
        Mutable,
        Immutable
    };

    enum ColorWriteMask_
    {
        ColorWriteMask_None = 0,
        ColorWriteMask_Red = 1 << 0,
        ColorWriteMask_Green = 1 << 1,
        ColorWriteMask_Blue = 1 << 2,
        ColorWriteMask_Alpha = 1 << 3,
        ColorWriteMask_All = ColorWriteMask_Red | ColorWriteMask_Green | ColorWriteMask_Blue | ColorWriteMask_Alpha
    };

    enum class BlendOperation
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max
    };

    enum class BlendFactor
    {
        Zero,
        One,
        SourceColor,
        OneMinusSourceColor,
        SourceAlpha,
        OneMinusSourceAlpha,
        DestinationColor,
        OneMinusDestinationColor,
        DestinationAlpha,
        OneMinusDestinationAlpha,
        SourceAlphaSaturated,
        BlendColor,
        OneMinusBlendColor,
        BlendAlpha,
        OneMinusBlendAlpha,
        Source1Color,
        OneMinusSource1Color,
        Source1Alpha,
        OneMinusSource1Alpha,
    };

    // this is currently very Metal centric and will need to
    // be refactored to better fit being an abstraction layer
    // that also works for vulkan.
    struct RenderPipelineDescriptor
    {
        // identifying the render pipeline state object
        std::string label;

        // graphics functions and associated data
        IShaderFunction* vertexFunction;
        IShaderFunction* fragmentFunction;
        int maxVertexCallstackDepth{1};
        int maxFragmentCallstackDepth{1};

        // buffer layout and fetch behavior
        VertexDescriptor vertexDescriptor;

        // buffer mutability (specifying this can improve performance)
        struct BufferDescriptor
        {
            Mutability mutability{Mutability::Default};
        };
        std::vector<BufferDescriptor> vertexBuffers;
        std::vector<BufferDescriptor> fragmentBuffers;

        // rendering pipeline state
        struct ColorAttachmentDescriptor
        {
            // render pipeline state
            PixelFormat pixelFormat;
            ColorWriteMask_ writeMask;

            // controlling blend operation
            bool blendingEnabled;
            BlendOperation alphaBlendOperation;
            BlendOperation rgbBlendOperation;

            // blend factors
            BlendFactor destinationAlphaBlendFactor;
            BlendFactor destinationRGBBlendFactor;
            BlendFactor sourceAlphaBlendFactor;
            BlendFactor sourceRGBBlendFactor;
        };
        std::vector<ColorAttachmentDescriptor> colorAttachments{};
        PixelFormat depthAttachmentPixelFormat;
        PixelFormat stencilAttachmentPixelFormat;

        // rasterization and visibility state
        bool alphaToCoverageEnabled{false};
        bool alphaToOneEnabled{false};
        bool rasterizationEnabled{true};
        PrimitiveTopologyType inputPrimitiveTopology{PrimitiveTopologyType::Unspecified};
        unsigned int rasterSampleCount{1};

        // tessellation state
        unsigned int maxTessellationFactor{16}; // max is 64
        bool tessellationFactorScaleEnabled{false};
        TessellationFactorFormat tessellationFactorFormat{TessellationFactorFormat::Half};
        TessellationControlPointIndexType tessellationControlPointIndexType{TessellationControlPointIndexType::None};
        TessellationFactorStepFunction tessellationFactorStepFunction;
        WindingOrder tessellationOutputWindingOrder{WindingOrder::Clockwise};
        TessellationPartitionMode tessellationPartitionMode{TessellationPartitionMode::Pow2};

        // indirect command buffers usage
        bool supportIndirectCommandBuffers{false};

        // maximum vertex amplification count
        unsigned int maxVertexAmplificationCount{1}; // 1 disables vertex amplification

        // specifying precompiled shader binaries
//        bool supportAddingVertexBinaryFunctions;
//        bool supportAddingFragmentBinaryFunctions;
//        int binaryArchives;
//
//        // callable functions for pipeline
//        int vertexLinkedFunctions;
//        int fragmentLinkedFunctions;
    };

    class IRenderPipelineState
    {
    public:
        virtual ~IRenderPipelineState() = default;
    };

    struct StencilDescriptor
    {
        // todo
    };

    struct DepthStencilDescriptor
    {
        // depth operations
        CompareFunction depthCompareFunction;
        bool depthWriteEnabled;

        // stencil descriptors for primitives
        // todo
    };

    class IDepthStencilState
    {
    public:
        virtual ~IDepthStencilState() = default;
    };
}

#endif //SHAPEREALITY_RENDER_PIPELINE_STATE_H
