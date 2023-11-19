//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_TEXTURE_H
#define BORED_ENGINE_MTL_TEXTURE_H

#include "../../texture.h"

namespace renderer
{
	class MetalTextureImplementation : public TextureImplementation
	{
	public:
		explicit MetalTextureImplementation(Texture* texture);
		~MetalTextureImplementation() override;

	private:
		//MTL::Texture* pTexture{nullptr};
	};
}

#endif //BORED_ENGINE_MTL_TEXTURE_H
