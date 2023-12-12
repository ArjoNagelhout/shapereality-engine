//
// Created by Arjo Nagelhout on 12/12/2023.
//

#include "shader.h"

#include <cassert>

namespace graphics
{
	std::unique_ptr<IShaderModule> IShaderLibrary::createShaderModule(ShaderModuleDescriptor descriptor)
	{
		assert(false && "interface class method should not be called");
	}
}