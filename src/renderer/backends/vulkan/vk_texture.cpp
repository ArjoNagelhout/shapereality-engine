//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "vk_texture.h"

namespace renderer
{
	VulkanTextureImplementation::VulkanTextureImplementation(Texture* texture) : TextureImplementation(texture)
	{
	}

	VulkanTextureImplementation::~VulkanTextureImplementation() = default;

}