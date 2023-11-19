//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_TEXTURE_H
#define BORED_ENGINE_MTL_TEXTURE_H

#include "../../texture.h"

namespace renderer
{
	class MetalTexture : public TextureImplementation
	{
	public:
		explicit MetalTexture(Texture* texture);
		~MetalTexture() override;

		class Implementation;

	private:
		std::unique_ptr<Implementation> pImplementation;
	};
}

#endif //BORED_ENGINE_MTL_TEXTURE_H
