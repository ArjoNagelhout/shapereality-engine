//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_TEXTURE_H
#define BORED_ENGINE_MTL_TEXTURE_H

#include "../../texture.h"
#include <Metal/Metal.hpp>

namespace renderer
{
	class MetalTexture : public Texture
	{
	public:
		explicit MetalTexture();
		~MetalTexture() override;

	private:
		MTL::Texture* pTexture{nullptr};
	};
}

#endif //BORED_ENGINE_MTL_TEXTURE_H
