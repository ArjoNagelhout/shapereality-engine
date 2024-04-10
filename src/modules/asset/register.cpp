//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "register.h"

#include <asset/asset_id.h>
#include <asset/asset_database.h>

#include <reflection/type_registry.h>
#include <reflection/serialize/json.h>
#include <reflection/class.h>

namespace asset
{
    void pathFromJson(nlohmann::json const& in, std::filesystem::path* out)
    {
        *out = in.get<std::string>();
    }

    void pathToJson(std::filesystem::path* in, nlohmann::json& out)
    {
        out = in->generic_string();
    }

    void fileTimeFromJson(nlohmann::json const& in, std::filesystem::file_time_type* out)
    {
        unsigned int timeSinceEpoch = in.get<unsigned int>();
        *out = std::filesystem::file_time_type() + std::filesystem::file_time_type::duration(timeSinceEpoch);
    }

    void fileTimeToJson(std::filesystem::file_time_type* in, nlohmann::json& out)
    {
        out = static_cast<unsigned int>(in->time_since_epoch().count());
    }

    void register_(reflection::Reflection& reflection)
    {
        reflection.types.emplace<std::filesystem::path>(std::make_unique<reflection::PrimitiveInfo>("Path"));
        reflection.json.emplace<std::filesystem::path>(pathFromJson, pathToJson);

        reflection.types.emplace<std::filesystem::file_time_type>(std::make_unique<reflection::PrimitiveInfo>("FileTimeType"));
        reflection.json.emplace<std::filesystem::file_time_type>(fileTimeFromJson, fileTimeToJson);

        reflection::register_::Class<ImportResultCache>("ImportResultCache")
            .member<&ImportResultCache::inputFilePath>("inputFilePath")
            .member<&ImportResultCache::lastWriteTime>("lastWriteTime")
            .member<&ImportResultCache::dependencies>("dependencies")
            .member<&ImportResultCache::artifacts>("artifacts")
            .emplace(reflection.types);

        reflection::register_::Class<AssetId>("AssetId")
            .member<&AssetId::inputFilePath>("inputFilePath")
            .member<&AssetId::artifactPath>("artifactPath")
            .emplace(reflection.types);
    }
}