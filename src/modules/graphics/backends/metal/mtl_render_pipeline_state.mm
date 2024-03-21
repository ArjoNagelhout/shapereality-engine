//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_render_pipeline_state.h"
#import "mtl_shader.h"

#include "mtl_utils.h"
#include "mtl_types.h"

#include <common/platform/apple/apple.h>

namespace graphics::metal
{
    MTLPrimitiveTopologyClass convert(PrimitiveTopologyType type)
    {
        switch (type)
        {
            case PrimitiveTopologyType::Unspecified: return MTLPrimitiveTopologyClassUnspecified;
            case PrimitiveTopologyType::Point: return MTLPrimitiveTopologyClassPoint;
            case PrimitiveTopologyType::Line: return MTLPrimitiveTopologyClassLine;
            case PrimitiveTopologyType::Triangle: return MTLPrimitiveTopologyClassTriangle;
        }
    }

    MTLVertexFormat convert(VertexFormat value)
    {
        switch (value)
        {
            case VertexFormat::Invalid: return MTLVertexFormatInvalid;
            case VertexFormat::UChar2: return MTLVertexFormatUChar2;
            case VertexFormat::UChar3: return MTLVertexFormatUChar3;
            case VertexFormat::UChar4: return MTLVertexFormatUChar4;
            case VertexFormat::Char2: return MTLVertexFormatChar2;
            case VertexFormat::Char3: return MTLVertexFormatChar3;
            case VertexFormat::Char4: return MTLVertexFormatChar4;
            case VertexFormat::UChar2Normalized: return MTLVertexFormatUChar2Normalized;
            case VertexFormat::UChar3Normalized: return MTLVertexFormatUChar3Normalized;
            case VertexFormat::UChar4Normalized: return MTLVertexFormatUChar4Normalized;
            case VertexFormat::Char2Normalized: return MTLVertexFormatChar2Normalized;
            case VertexFormat::Char3Normalized: return MTLVertexFormatChar3Normalized;
            case VertexFormat::Char4Normalized: return MTLVertexFormatChar4Normalized;
            case VertexFormat::UShort2: return MTLVertexFormatUShort2;
            case VertexFormat::UShort3: return MTLVertexFormatUShort3;
            case VertexFormat::UShort4: return MTLVertexFormatUShort4;
            case VertexFormat::Short2: return MTLVertexFormatShort2;
            case VertexFormat::Short3: return MTLVertexFormatShort3;
            case VertexFormat::Short4: return MTLVertexFormatShort4;
            case VertexFormat::UShort2Normalized: return MTLVertexFormatUShort2Normalized;
            case VertexFormat::UShort3Normalized: return MTLVertexFormatUShort3Normalized;
            case VertexFormat::UShort4Normalized: return MTLVertexFormatUShort4Normalized;
            case VertexFormat::Short2Normalized: return MTLVertexFormatShort2Normalized;
            case VertexFormat::Short3Normalized: return MTLVertexFormatShort3Normalized;
            case VertexFormat::Short4Normalized: return MTLVertexFormatShort4Normalized;
            case VertexFormat::Half2: return MTLVertexFormatHalf2;
            case VertexFormat::Half3: return MTLVertexFormatHalf3;
            case VertexFormat::Half4: return MTLVertexFormatHalf4;
            case VertexFormat::Float: return MTLVertexFormatFloat;
            case VertexFormat::Float2: return MTLVertexFormatFloat2;
            case VertexFormat::Float3: return MTLVertexFormatFloat3;
            case VertexFormat::Float4: return MTLVertexFormatFloat4;
            case VertexFormat::Int: return MTLVertexFormatInt;
            case VertexFormat::Int2: return MTLVertexFormatInt2;
            case VertexFormat::Int3: return MTLVertexFormatInt3;
            case VertexFormat::Int4: return MTLVertexFormatInt4;
            case VertexFormat::UInt: return MTLVertexFormatUInt;
            case VertexFormat::UInt2: return MTLVertexFormatUInt2;
            case VertexFormat::UInt3: return MTLVertexFormatUInt3;
            case VertexFormat::UInt4: return MTLVertexFormatUInt4;
            case VertexFormat::Int1010102Normalized: return MTLVertexFormatInt1010102Normalized;
            case VertexFormat::UInt1010102Normalized: return MTLVertexFormatUInt1010102Normalized;
            case VertexFormat::UChar4Normalized_BGRA: return MTLVertexFormatUChar4Normalized_BGRA;
            case VertexFormat::UChar: return MTLVertexFormatUChar;
            case VertexFormat::Char: return MTLVertexFormatChar;
            case VertexFormat::UCharNormalized: return MTLVertexFormatUCharNormalized;
            case VertexFormat::CharNormalized: return MTLVertexFormatCharNormalized;
            case VertexFormat::UShort: return MTLVertexFormatUShort;
            case VertexFormat::Short: return MTLVertexFormatShort;
            case VertexFormat::UShortNormalized: return MTLVertexFormatUShortNormalized;
            case VertexFormat::ShortNormalized: return MTLVertexFormatShortNormalized;
            case VertexFormat::Half: return MTLVertexFormatHalf;
            case VertexFormat::FloatRG11B10: return MTLVertexFormatFloatRG11B10;
            case VertexFormat::FloatRGB9E5: return MTLVertexFormatFloatRGB9E5;
        }
    }

    MTLVertexStepFunction convert(VertexStepFunction value)
    {
        switch (value)
        {
            case VertexStepFunction::Constant: return MTLVertexStepFunctionConstant;
            case VertexStepFunction::PerVertex: return MTLVertexStepFunctionPerVertex;
            case VertexStepFunction::PerInstance: return MTLVertexStepFunctionPerInstance;
            case VertexStepFunction::PerPatch: return MTLVertexStepFunctionPerPatch;
            case VertexStepFunction::PerPatchControlPoint: return MTLVertexStepFunctionPerPatchControlPoint;
        }
    }

    MTLTessellationFactorFormat convert(TessellationFactorFormat format)
    {
        switch (format)
        {
            case TessellationFactorFormat::Half: return MTLTessellationFactorFormatHalf;
        }
    }

    MTLTessellationControlPointIndexType convert(TessellationControlPointIndexType value)
    {
        switch (value)
        {
            case TessellationControlPointIndexType::None: return MTLTessellationControlPointIndexTypeNone;
            case TessellationControlPointIndexType::UInt16: return MTLTessellationControlPointIndexTypeUInt16;
            case TessellationControlPointIndexType::UInt32: return MTLTessellationControlPointIndexTypeUInt32;
        }
    }

    MTLTessellationPartitionMode convert(TessellationPartitionMode mode)
    {
        switch (mode)
        {
            case TessellationPartitionMode::Pow2: return MTLTessellationPartitionModePow2;
            case TessellationPartitionMode::Integer: return MTLTessellationPartitionModeInteger;
            case TessellationPartitionMode::FractionalOdd: return MTLTessellationPartitionModeFractionalOdd;
            case TessellationPartitionMode::FractionalEven: return MTLTessellationPartitionModeFractionalEven;
        }
    }

    MTLTessellationFactorStepFunction convert(TessellationFactorStepFunction value)
    {
        switch (value)
        {
            case TessellationFactorStepFunction::Constant: return MTLTessellationFactorStepFunctionConstant;
        }
    }

    MTLMutability convert(Mutability value)
    {
        switch (value)
        {
            case Mutability::Default: return MTLMutabilityDefault;
            case Mutability::Mutable: return MTLMutabilityMutable;
            case Mutability::Immutable: return MTLMutabilityImmutable;
        }
    }

    // todo: ColorWriteMask_

    MTLBlendOperation convert(BlendOperation value)
    {
        switch (value)
        {
            case BlendOperation::Add: return MTLBlendOperationAdd;
            case BlendOperation::Subtract: return MTLBlendOperationSubtract;
            case BlendOperation::ReverseSubtract: return MTLBlendOperationReverseSubtract;
            case BlendOperation::Min: return MTLBlendOperationMin;
            case BlendOperation::Max: return MTLBlendOperationMax;
        }
    }

    MTLBlendFactor convert(BlendFactor value)
    {
        switch (value)
        {
            case BlendFactor::Zero: return MTLBlendFactorZero;
            case BlendFactor::One: return MTLBlendFactorOne;
            case BlendFactor::SourceColor: return MTLBlendFactorSourceColor;
            case BlendFactor::OneMinusSourceColor: return MTLBlendFactorOneMinusSourceColor;
            case BlendFactor::SourceAlpha: return MTLBlendFactorSourceAlpha;
            case BlendFactor::OneMinusSourceAlpha: return MTLBlendFactorOneMinusSourceAlpha;
            case BlendFactor::DestinationColor: return MTLBlendFactorDestinationColor;
            case BlendFactor::OneMinusDestinationColor: return MTLBlendFactorOneMinusDestinationColor;
            case BlendFactor::DestinationAlpha: return MTLBlendFactorDestinationAlpha;
            case BlendFactor::OneMinusDestinationAlpha: return MTLBlendFactorOneMinusDestinationAlpha;
            case BlendFactor::SourceAlphaSaturated: return MTLBlendFactorSourceAlphaSaturated;
            case BlendFactor::BlendColor: return MTLBlendFactorBlendColor;
            case BlendFactor::OneMinusBlendColor: return MTLBlendFactorOneMinusBlendColor;
            case BlendFactor::BlendAlpha: return MTLBlendFactorBlendAlpha;
            case BlendFactor::OneMinusBlendAlpha: return MTLBlendFactorOneMinusBlendAlpha;
            case BlendFactor::Source1Color: return MTLBlendFactorSource1Color;
            case BlendFactor::OneMinusSource1Color: return MTLBlendFactorOneMinusSource1Color;
            case BlendFactor::Source1Alpha: return MTLBlendFactorSource1Alpha;
            case BlendFactor::OneMinusSource1Alpha: return MTLBlendFactorOneMinusSource1Alpha;
        }
    }

    MTLVertexDescriptor* createVertexDescriptor(VertexDescriptor* descriptor)
    {
        if (descriptor == nullptr)
        {
            return nullptr;
        }

        MTLVertexDescriptor* result = [[MTLVertexDescriptor alloc] init];

        for (size_t i = 0; i < descriptor->attributes.size(); i++)
        {
            VertexAttributeDescriptor const& source = descriptor->attributes[i];
            MTLVertexAttributeDescriptor* attribute = [[MTLVertexAttributeDescriptor alloc] init];
            attribute.format = convert(source.format);
            attribute.offset = source.offset;
            attribute.bufferIndex = source.bufferIndex;
            [result.attributes setObject:attribute atIndexedSubscript:i];
        }

        for (size_t i = 0; i < descriptor->layouts.size(); i++)
        {
            VertexBufferLayoutDescriptor const& source = descriptor->layouts[i];
            MTLVertexBufferLayoutDescriptor* layout = [[MTLVertexBufferLayoutDescriptor alloc] init];
            layout.stepFunction = convert(source.stepFunction);
            layout.stepRate = source.stepRate;
            layout.stride = source.stride;
            [result.layouts setObject:layout atIndexedSubscript:i];
        }

        return result;
    }

    MetalRenderPipelineState::MetalRenderPipelineState(id <MTLDevice> _Nonnull device,
                                                       RenderPipelineDescriptor const& descriptor)
    {
        MTLRenderPipelineDescriptor* d = [[MTLRenderPipelineDescriptor alloc] init];

        // identifying the render pipeline state object
        d.label = common::toNSString(descriptor.label);

        // graphics functions and associated data
        d.vertexFunction = dynamic_cast<MetalShaderFunction*>(descriptor.vertexFunction)->get();
        d.fragmentFunction = dynamic_cast<MetalShaderFunction*>(descriptor.fragmentFunction)->get();
        d.maxVertexCallStackDepth = descriptor.maxVertexCallstackDepth;
        d.maxFragmentCallStackDepth = descriptor.maxFragmentCallstackDepth;

        // buffer layout and fetch behavior
        d.vertexDescriptor = createVertexDescriptor(descriptor.vertexDescriptor.get());

        // buffer mutability (specifying this can improve performance)
        for (size_t i = 0; i < descriptor.vertexBuffers.size(); i++)
        {
            RenderPipelineDescriptor::BufferDescriptor const& source = descriptor.vertexBuffers[i];
            MTLPipelineBufferDescriptor* buffer = [[MTLPipelineBufferDescriptor alloc] init];
            buffer.mutability = convert(source.mutability);
            [d.vertexBuffers setObject:buffer atIndexedSubscript:i];
        }

        for (size_t i = 0; i < descriptor.fragmentBuffers.size(); i++)
        {
            RenderPipelineDescriptor::BufferDescriptor const& source = descriptor.fragmentBuffers[i];
            MTLPipelineBufferDescriptor* buffer = [[MTLPipelineBufferDescriptor alloc] init];
            buffer.mutability = convert(source.mutability);
            [d.fragmentBuffers setObject:buffer atIndexedSubscript:i];
        }

        // rendering pipeline state
        for (size_t i = 0; i < descriptor.colorAttachments.size(); i++)
        {
            RenderPipelineDescriptor::ColorAttachmentDescriptor const& source = descriptor.colorAttachments[i];
            MTLRenderPipelineColorAttachmentDescriptor* color = [[MTLRenderPipelineColorAttachmentDescriptor alloc] init];
            color.pixelFormat = convert(source.pixelFormat);
            // color.writeMask = convert(source.writeMask); // TODO
            color.blendingEnabled = source.blendingEnabled;
            color.alphaBlendOperation = convert(source.alphaBlendOperation);
            color.rgbBlendOperation = convert(source.rgbBlendOperation);
            color.destinationAlphaBlendFactor = convert(source.destinationAlphaBlendFactor);
            color.destinationRGBBlendFactor = convert(source.destinationRGBBlendFactor);
            color.sourceAlphaBlendFactor = convert(source.sourceAlphaBlendFactor);
            color.sourceRGBBlendFactor = convert(source.sourceRGBBlendFactor);
            [d.colorAttachments setObject:color atIndexedSubscript:i];
        }

        d.depthAttachmentPixelFormat = convert(descriptor.depthAttachmentPixelFormat);
        d.stencilAttachmentPixelFormat = convert(descriptor.stencilAttachmentPixelFormat);

        // rasterization and visibility state
        d.alphaToCoverageEnabled = descriptor.alphaToCoverageEnabled;
        d.alphaToOneEnabled = descriptor.alphaToOneEnabled;
        d.rasterizationEnabled = descriptor.rasterizationEnabled;
        d.inputPrimitiveTopology = convert(descriptor.inputPrimitiveTopology);
        d.rasterSampleCount = descriptor.rasterSampleCount;

        // tessellation state
        d.maxTessellationFactor = descriptor.maxTessellationFactor;
        d.tessellationFactorScaleEnabled = descriptor.tessellationFactorScaleEnabled;
        d.tessellationFactorFormat = convert(descriptor.tessellationFactorFormat);
        d.tessellationControlPointIndexType = convert(descriptor.tessellationControlPointIndexType);
        d.tessellationFactorStepFunction = convert(descriptor.tessellationFactorStepFunction);
        d.tessellationOutputWindingOrder = convert(descriptor.tessellationOutputWindingOrder);
        d.tessellationPartitionMode = convert(descriptor.tessellationPartitionMode);

        // indirect command buffers usage
        d.supportIndirectCommandBuffers = descriptor.supportIndirectCommandBuffers;

        // maximum vertex amplification count
        d.maxVertexAmplificationCount = descriptor.maxVertexAmplificationCount;

        // create from converted render pipeline descriptor
        NSError* error = nullptr;
        renderPipelineState = [device newRenderPipelineStateWithDescriptor:d error:&error];
        checkMetalError(error, "failed to create MTLRenderPipelineState, for some reason");

        [renderPipelineState retain];
    }

    MetalRenderPipelineState::~MetalRenderPipelineState()
    {
        [renderPipelineState release];
    }

    id <MTLRenderPipelineState> _Nonnull MetalRenderPipelineState::get() const
    {
        return renderPipelineState;
    }

    MetalDepthStencilState::MetalDepthStencilState(id <MTLDevice> _Nonnull device,
                                                   DepthStencilDescriptor const& descriptor)
    {
        MTLDepthStencilDescriptor* metalDescriptor = [[MTLDepthStencilDescriptor alloc] init];

        metalDescriptor.depthCompareFunction = convert(descriptor.depthCompareFunction);
        metalDescriptor.depthWriteEnabled = descriptor.depthWriteEnabled;

        depthStencilState = [device newDepthStencilStateWithDescriptor:metalDescriptor];
        [depthStencilState retain];
    }

    MetalDepthStencilState::~MetalDepthStencilState()
    {
        [depthStencilState release];
    }

    id <MTLDepthStencilState> _Nonnull MetalDepthStencilState::get() const
    {
        return depthStencilState;
    }
}