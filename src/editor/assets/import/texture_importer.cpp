//
// Created by Arjo Nagelhout on 22/12/2023.
//

#include "texture_importer.h"

#include "lodepng.h"
#include <iostream>

namespace assets
{
	TextureImportResult importTexture(graphics::IDevice* pDevice,
									  std::filesystem::path const& source,
									  TextureImportDescriptor const& descriptor,
									  std::unique_ptr<graphics::ITexture>& outTexture)
	{
		TextureImportResult result{};

		std::vector<unsigned char> image;
		unsigned int width;
		unsigned int height;
		unsigned int error = lodepng::decode(image, width, height, source.c_str());

		if (error != 0)
		{
			result.success = false;
			result.errorMessage = lodepng_error_text(error);
			return result;
		}

		std::cout << "width: " << width << ", height: " << height << std::endl;

		graphics::TextureDescriptor textureDescriptor{
			.width = width,
			.height = height,
			.pixelFormat = graphics::PixelFormat::RGBA8Unorm_sRGB,
			.usage = graphics::TextureUsage_ShaderRead
		};

		outTexture = pDevice->createTexture(textureDescriptor);

		result.success = true;
		return result;
	}
}