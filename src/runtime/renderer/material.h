//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef SHAPEREALITY_MATERIAL_H
#define SHAPEREALITY_MATERIAL_H

#include "graphics/texture.h"
#include "graphics/shader.h"

namespace renderer
{
	class Material
	{
	public:
		explicit Material();
		~Material();

	private:
		graphics::ITexture* pTexture; // unowned pointer
	};
}

#endif //SHAPEREALITY_MATERIAL_H
