//
// Created by Arjo Nagelhout on 18/11/2023.
//

#include "../vk_texture.h"

namespace renderer
{
	VulkanTexture::VulkanTexture(Texture* texture) : TextureImplementation(texture)
	{
	}

	VulkanTexture::~VulkanTexture() = default;

}