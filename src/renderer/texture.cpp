#include "texture.h"

namespace renderer
{
	Texture::Texture(renderer::TextureFormat textureFormat) : textureFormat(textureFormat)
	{
	}

	Texture::~Texture() = default;
}