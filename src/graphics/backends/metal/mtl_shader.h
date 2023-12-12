//
// Created by Arjo Nagelhout on 12/12/2023.
//

#ifndef BORED_ENGINE_MTL_SHADER_H
#define BORED_ENGINE_MTL_SHADER_H

#include "../../shader.h"

#import <Metal/Metal.h>

namespace graphics
{
	class MetalShaderModule final : public IShaderModule
	{
	public:
		explicit MetalShaderModule(ShaderModuleDescriptor descriptor, id<MTLLibrary> _Nonnull pLibrary);
		~MetalShaderModule() override;
	};

	class MetalShaderLibrary final : public IShaderLibrary
	{
	public:
		explicit MetalShaderLibrary(id<MTLDevice> _Nonnull pDevice);
		~MetalShaderLibrary() override;

		[[nodiscard]] std::unique_ptr<IShaderModule> createShaderModule(ShaderModuleDescriptor descriptor) override;

	private:
		id<MTLLibrary> _Nonnull pLibrary;
	};
}

#endif //BORED_ENGINE_MTL_SHADER_H
