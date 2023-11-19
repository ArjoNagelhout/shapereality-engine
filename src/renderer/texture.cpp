#include "texture.h"

#include "backends/metal/mtl_texture.h"
#include "backends/vulkan/vk_texture.h"

namespace renderer
{
	TextureImplementation::TextureImplementation(Texture* texture) : pTexture(texture)
	{
	}

	TextureImplementation::~TextureImplementation() = default;

	Texture::Texture(uint32_t const& width, uint32_t const& height, TextureFormat const& format)
		: width(width), height(height), format(format)
	{
	}

	Texture::~Texture() = default;

	void Texture::onRendererBackendChanged(RendererBackendType const& rendererBackendType)
	{
		switch (rendererBackendType)
		{
			case RendererBackendType::Metal:
				pImplementation = std::make_unique<MetalTexture>(this);
				break;
			case RendererBackendType::Vulkan:
				pImplementation = std::make_unique<VulkanTexture>(this);
				break;
			default:
				pImplementation.reset();
		}
	}

	TextureFormat Texture::getFormat()
	{
		return format;
	}

	uint32_t Texture::getWidth() const
	{
		return width;
	}

	uint32_t Texture::getHeight() const
	{
		return height;
	}
}