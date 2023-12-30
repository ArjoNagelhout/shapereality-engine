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

        std::vector<unsigned char> png;
        unsigned int width;
        unsigned int height;
        lodepng::State state;

        lodepng::load_file(png, source.c_str());

        std::vector<unsigned char> image;
        unsigned int error = lodepng::decode(image, width, height, state, png);

        if (error != 0)
        {
            result.success = false;
            result.errorMessage = lodepng_error_text(error);
            return result;
        }

        std::cout << "width: " << width
                  << "\nheight: " << height
                  << "\nbitdepth: " << state.info_raw.bitdepth // 8
                  << "\ncolortype: " << state.info_raw.colortype // 6 = RGBA
                  << "\nkey_b: " << state.info_raw.key_b
                  << "\nkey_defined: " << state.info_raw.key_defined
                  << "\nkey_g: " << state.info_raw.key_g
                  << "\nkey_r: " << state.info_raw.key_r
//                  << "\npalette: " << state.info_raw.palette
//                  << "\npalettesize: " << state.info_raw.palettesize
                  << std::endl;

        graphics::TextureDescriptor textureDescriptor{
            .width = width,
            .height = height,
            .pixelFormat = graphics::PixelFormat::RGBA8Unorm_sRGB,
            .usage = graphics::TextureUsage_ShaderRead,
            .data = image.data()
        };

        outTexture = pDevice->createTexture(textureDescriptor);

        result.success = true;
        return result;
    }
}