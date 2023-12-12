//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef BORED_ENGINE_SHADER_H
#define BORED_ENGINE_SHADER_H

#include <memory>
#include <string>

namespace graphics
{
	struct ShaderModuleDescriptor
	{
		std::string entryPoint;
//		std::string specializedName;
//		int constantValues;
//		int options;
	};

	// a shader library contains a set of shader modules.
	// This is shader source code that contains either a vertex shader or a fragment shader.
	class IShaderModule
	{
	public:
		virtual ~IShaderModule() = default;
	};

	// contains shader source code, either loaded from a file
	// in Metal, this wraps an MTLLibrary, containing shader source code accessible by string
	// in Vulkan, this contains a list of VkShaderModules.
	class IShaderLibrary
	{
	public:
		virtual ~IShaderLibrary() = default;

		[[nodiscard]] virtual std::unique_ptr<IShaderModule> createShaderModule(ShaderModuleDescriptor descriptor);
	};
}

#endif //BORED_ENGINE_SHADER_H
