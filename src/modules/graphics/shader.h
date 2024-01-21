//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef SHAPEREALITY_SHADER_H
#define SHAPEREALITY_SHADER_H

#include <memory>
#include <string>

namespace graphics
{
    // whether a shader function is used for vertex or fragment
    enum class ShaderFunctionType
    {
        Vertex,
        Fragment
    };

    struct ShaderFunctionDescriptor
    {
        std::string entryPoint; // the name of the function to use / load from the shader library
        ShaderFunctionType type;
//        int constantValues;
//        int options;
    };

    // a shader library contains a set of shader functions.
    // These shader functions are either vertex or fragment functions
    class IShaderFunction
    {
    public:
        virtual ~IShaderFunction() = default;
    };

    // contains shader source code, either loaded from a file
    // in Metal, this wraps an MTLLibrary, containing shader source code accessible by string
    // in Vulkan, this contains a list of VkShaderModules.
    class IShaderLibrary
    {
    public:
        virtual ~IShaderLibrary() = default;

        [[nodiscard]] virtual std::unique_ptr<IShaderFunction>
        createShaderFunction(ShaderFunctionDescriptor const& descriptor) = 0;
    };
}

#endif //SHAPEREALITY_SHADER_H
