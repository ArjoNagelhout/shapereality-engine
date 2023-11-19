//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_SHADER_H
#define BORED_ENGINE_SHADER_H

#include <map>
#include <string>
#include <vector>
#include <filesystem>

namespace renderer
{
	class ShaderProperty
	{

	};

	class IntegerProperty : public ShaderProperty
	{

	};

	class FloatProperty : public ShaderProperty
	{

	};

	class Texture : public ShaderProperty
	{

	};

	class Color : public ShaderProperty
	{

	};

	class Vector : public ShaderProperty
	{

	};

	// shaders should be written in a certain language (HLSL, GLSL) and compiled to the native shader code (such as Spir-V)
	// we should also be able to load the properties from the file
	// https://developer.apple.com/documentation/metal/shader_libraries/building_a_shader_library_by_precompiling_source_files?language=objc

	// https://alain.xyz/blog/a-review-of-shader-languages
	class Shader
	{
	public:
		explicit Shader(std::filesystem::path const& path);
		~Shader();

		void compile();

	private:
		std::map<std::string, std::unique_ptr<ShaderProperty>> pProperties{};
	};
}

#endif //BORED_ENGINE_SHADER_H
