//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "reflection.h"

#include <asset/asset_id.h>
#include <asset/asset_database.h>

namespace asset
{
    void pathFromJson(nlohmann::json const& in, fs::path* out)
    {
        *out = in.get<std::string>();
    }

    void pathToJson(fs::path* in, nlohmann::json& out)
    {
        out = in->generic_string();
    }

    void fileTimeFromJson(nlohmann::json const& in, fs::file_time_type* out)
    {
        unsigned int timeSinceEpoch = in.get<unsigned int>();
        *out = fs::file_time_type() + fs::file_time_type::duration(timeSinceEpoch);
    }

    void fileTimeToJson(fs::file_time_type* in, nlohmann::json& out)
    {
        out = static_cast<unsigned int>(in->time_since_epoch().count());
    }

    void registerReflection()
    {
        reflection::TypeInfoRegistry& r = reflection::TypeInfoRegistry::shared();
        reflection::JsonSerializer& jsonSerializer = reflection::JsonSerializer::shared();

        r.emplace<fs::path>({"Path"});
        jsonSerializer.emplace<fs::path>(pathFromJson, pathToJson);

        r.emplace<fs::file_time_type>({"FileTimeType"});
        jsonSerializer.emplace<fs::file_time_type>(fileTimeFromJson, fileTimeToJson);

        reflection::TypeInfoBuilder<ImportResultCache>("ImportResultCache")
            .property<&ImportResultCache::inputFilePath>("inputFilePath")
            .property<&ImportResultCache::lastWriteTime>("lastWriteTime")
            .property<&ImportResultCache::artifactPaths>("artifactPaths")
            .emplace(r);

        reflection::TypeInfoBuilder<AssetId>("AssetId")
            .property<&AssetId::inputFilePath>("inputFilePath")
            .property<&AssetId::artifactPath>("artifactPath")
            .emplace(r);
    }
}