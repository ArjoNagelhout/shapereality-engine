//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef BORED_ENGINE_MTL_SHADER_H
#define BORED_ENGINE_MTL_SHADER_H

#include "../../shader.h"

#include <filesystem>

#import <Metal/Metal.h>

namespace graphics
{
	class MetalShaderFunction final : public IShaderFunction
	{
	public:
		explicit MetalShaderFunction(ShaderFunctionDescriptor const& descriptor, id<MTLLibrary> _Nonnull pLibrary);
		~MetalShaderFunction() override;
	};

	class MetalShaderLibrary final : public IShaderLibrary
	{
	public:
		explicit MetalShaderLibrary(std::filesystem::path const& path, id<MTLDevice> _Nonnull pDevice);
		~MetalShaderLibrary() override;

		[[nodiscard]] std::unique_ptr<IShaderFunction> createShaderFunction(ShaderFunctionDescriptor descriptor) override;

	private:
		id<MTLLibrary> _Nonnull pLibrary;
	};
}

#endif //BORED_ENGINE_MTL_SHADER_H
