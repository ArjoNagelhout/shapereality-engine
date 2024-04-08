//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "reflection.h"

#include <asset/asset_id.h>
#include <asset/asset_database.h>
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

    void registerReflection()
    {
        reflection::TypeInfoRegistry& r = reflection::TypeInfoRegistry::shared();
        reflection::JsonSerializer& jsonSerializer = reflection::JsonSerializer::shared();

        r.emplace<std::filesystem::path>(std::make_unique<PrimitiveInfo>("Path"));
        jsonSerializer.emplace<std::filesystem::path>(pathFromJson, pathToJson);

        r.emplace<std::filesystem::file_time_type>(std::make_unique<PrimitiveInfo>("FileTimeType"));
        jsonSerializer.emplace<std::filesystem::file_time_type>(fileTimeFromJson, fileTimeToJson);

        reflection::ClassInfoBuilder<ImportResultCache>("ImportResultCache")
            .member<&ImportResultCache::inputFilePath>("inputFilePath")
            .member<&ImportResultCache::lastWriteTime>("lastWriteTime")
            .member<&ImportResultCache::dependencies>("dependencies")
            .member<&ImportResultCache::artifacts>("artifacts")
            .emplace(r);

        reflection::ClassInfoBuilder<AssetId>("AssetId")
            .member<&AssetId::inputFilePath>("inputFilePath")
            .member<&AssetId::artifactPath>("artifactPath")
            .emplace(r);
    }
}