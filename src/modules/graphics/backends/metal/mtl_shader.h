//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef SHAPEREALITY_MTL_SHADER_H
#define SHAPEREALITY_MTL_SHADER_H

#include "graphics/shader.h"

#include <filesystem>

#import <Metal/Metal.h>

namespace graphics
{
    class MetalShaderFunction final : public IShaderFunction
    {
    public:
        explicit MetalShaderFunction(id <MTLLibrary> _Nonnull pLibrary, ShaderFunctionDescriptor const& descriptor);

        ~MetalShaderFunction() override;

        id <MTLFunction> _Nonnull getFunction();

    private:
        id <MTLFunction> _Nonnull pFunction;
    };

    class MetalShaderLibrary final : public IShaderLibrary
    {
    public:
        explicit MetalShaderLibrary(id <MTLDevice> _Nonnull pDevice, std::filesystem::path const& path);

        ~MetalShaderLibrary() override;

        [[nodiscard]] std::unique_ptr<IShaderFunction>
        createShaderFunction(ShaderFunctionDescriptor const& descriptor) override;

    private:
        id <MTLLibrary> _Nonnull pLibrary;
    };
}

#endif //SHAPEREALITY_MTL_SHADER_H
