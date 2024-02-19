//
// Created by Arjo Nagelhout on 22/12/2023.
//

#ifndef SHAPEREALITY_TEXTURE_IMPORTER_H
#define SHAPEREALITY_TEXTURE_IMPORTER_H

#include "graphics/device.h"
#include "graphics/texture.h"

namespace asset
{
    struct TextureImportDescriptor
    {

    };

    struct TextureImportResult
    {
        bool success;
        char const* errorMessage;
    };

    [[nodiscard]] TextureImportResult importTexture(graphics::IDevice* device,
                                                    std::filesystem::path const& source,
                                                    TextureImportDescriptor const& descriptor,
                                                    std::unique_ptr<graphics::ITexture>& outTexture);
}

#endif //SHAPEREALITY_TEXTURE_H
