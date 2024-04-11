//
// Created by Arjo Nagelhout on 27/12/2023.
//

#include "shader.h"
#include "graphics/device.h"

namespace renderer
{
    Shader::Shader(graphics::IDevice* device,
                   graphics::IShaderLibrary* shaderLibrary,
                   std::string const& vertexEntryPoint,
                   std::string const& fragmentEntryPoint)
    {
        graphics::ShaderFunctionDescriptor vertexDescriptor{
            .entryPoint = vertexEntryPoint,
            .type = graphics::ShaderFunctionType::Vertex
        };
        std::unique_ptr<graphics::IShaderFunction> pVertexFunction = shaderLibrary->createShaderFunction(
            vertexDescriptor);

        graphics::ShaderFunctionDescriptor fragmentDescriptor{
            .entryPoint = fragmentEntryPoint,
            .type = graphics::ShaderFunctionType::Fragment
        };
        std::unique_ptr<graphics::IShaderFunction> pFragmentFunction = shaderLibrary->createShaderFunction(
            fragmentDescriptor);

        graphics::RenderPipelineDescriptor renderPipelineDescriptor{
            .vertexFunction = pVertexFunction.get(),
            .fragmentFunction = pFragmentFunction.get(),
            .colorAttachments = {
                {.pixelFormat = graphics::PixelFormat::BGRA8Unorm_sRGB}
            },
            .depthAttachmentPixelFormat = graphics::PixelFormat::Depth16Unorm,
        };

        pRenderPipelineState = device->createRenderPipelineState(renderPipelineDescriptor);
    }

    Shader::~Shader() = default;

    graphics::IRenderPipelineState* Shader::getRenderPipelineState() const
    {
        return pRenderPipelineState.get();
    }
}