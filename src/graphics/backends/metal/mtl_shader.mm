//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "mtl_shader.h"

namespace graphics
{
//	MTLFunctionDescriptor convert(ShaderModuleDescriptor descriptor)
//	{
//
//	}

	MetalShaderModule::MetalShaderModule(ShaderModuleDescriptor descriptor, id<MTLLibrary> _Nonnull pLibrary)
	{

	}

	MetalShaderModule::~MetalShaderModule() = default;

	MetalShaderLibrary::MetalShaderLibrary(id<MTLDevice> _Nonnull pDevice)
	{
		pLibrary = [pDevice newDefaultLibrary];

		[pLibrary retain];
	}

	MetalShaderLibrary::~MetalShaderLibrary()
	{
		[pLibrary release];
	}

	std::unique_ptr<IShaderModule> MetalShaderLibrary::createShaderModule(ShaderModuleDescriptor descriptor)
	{
		return std::make_unique<MetalShaderModule>(descriptor, pLibrary);
	}
}