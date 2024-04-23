//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "png.h"

#include <iostream>
#include <asset/asset_database.h>
#include <graphics/texture.h>

#include "lodepng.h"

namespace import_::texture
{
    asset::ImportResult importPng(asset::AssetDatabase& assetDatabase, std::filesystem::path const& inputFile)
    {
        std::filesystem::path const path = assetDatabase.absolutePath(inputFile);
        asset::ImportResultData result;
        asset::AssetDatabaseContext const& context = assetDatabase.context();
        graphics::IDevice* device = context.device;

        std::vector<unsigned char> png;
        unsigned int width;
        unsigned int height;
        lodepng::State state;

        lodepng::load_file(png, path.c_str());

        std::vector<unsigned char> image;
        unsigned int error = lodepng::decode(image, width, height, state, png);

        if (error != 0)
        {
            return asset::ImportResult::makeError(common::ResultCode::Aborted, lodepng_error_text(error));
        }

        graphics::TextureDescriptor descriptor{
            .width = width,
            .height = height,
            .pixelFormat = graphics::PixelFormat::RGBA8Unorm_sRGB, // todo: make generic
            .usage = graphics::TextureUsage_ShaderRead,
            .data = image.data()
        };

        asset::AssetId id = context.assetTypes.makeAssetId<graphics::ITexture>(inputFile, "texture");
        asset::Asset asset = std::make_shared<asset::AssetHandle>(id);
        asset->set<graphics::ITexture>(device->createTexture(descriptor));

        result.artifacts.emplace_back(std::move(asset));

        return asset::ImportResult::makeSuccess(std::move(result));
        //return asset::ImportResult::makeError(common::ResultCode::Unimplemented, "whoops, todo");
    }
}