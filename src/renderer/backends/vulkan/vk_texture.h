//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_VK_TEXTURE_H
#define BORED_ENGINE_VK_TEXTURE_H

#include "../../texture.h"

namespace renderer
{
	class VulkanTexture final : public TextureImplementation
	{
	public:
		explicit VulkanTexture(Texture* texture);
		~VulkanTexture() override;

	private:

	};
}

#endif //BORED_ENGINE_VK_TEXTURE_H
