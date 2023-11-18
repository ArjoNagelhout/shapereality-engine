#include "texture.h"

#include "backends/metal/mtl_texture.h"
#include "backends/vulkan/vk_texture.h"

namespace renderer
{
	TextureImplementation::TextureImplementation(Texture* texture) : pTexture(texture)
	{
	}

	TextureImplementation::~TextureImplementation() = default;

	Texture::Texture(renderer::TextureFormat textureFormat) : textureFormat(textureFormat)
	{
	}

	Texture::~Texture() = default;

	void Texture::onRendererBackendChanged(RendererBackendType const& rendererBackendType)
	{
		switch (rendererBackendType)
		{
			case RendererBackendType::Metal:
				pImplementation = std::make_unique<MetalTextureImplementation>(this);
				break;
			case RendererBackendType::Vulkan:
				pImplementation = std::make_unique<VulkanTextureImplementation>(this);
				break;
			default:
				pImplementation.reset();
		}
	}
}