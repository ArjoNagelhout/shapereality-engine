//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef SHAPEREALITY_TEXTURE_H
#define SHAPEREALITY_TEXTURE_H

#include "graphics.h"

namespace graphics
{
	struct TextureDescriptor
	{
		PixelFormat pixelFormat;

		unsigned int width;
		unsigned int height;

		// data of the texture
		void const* data{nullptr};

		// size in bytes of the texture is calculated
		// from the width, height and pixel format,
		// so we don't have to specify it on texture creation

		// todo: add all texture properties
		// e.g. depth, storage mode, anti-aliasing
		// mip-mapping
	};

	class ITexture
	{
	public:
		virtual ~ITexture() = default;
	};
}

#endif //SHAPEREALITY_TEXTURE_H
