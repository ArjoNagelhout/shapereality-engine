//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_ASSET_REFLECTION_H
#define SHAPEREALITY_ASSET_REFLECTION_H

#include <reflection/type_info.h>
#include <reflection/serialize/json.h>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

namespace asset
{
    void pathFromJson(nlohmann::json const& in, fs::path* out);

    void pathToJson(fs::path* in, nlohmann::json& out);

    void fileTimeFromJson(nlohmann::json const& in, fs::file_time_type* out);

    void fileTimeToJson(fs::file_time_type* in, nlohmann::json& out);

    void registerReflection(reflection::TypeInfoRegistry& r, reflection::JsonSerializer& jsonSerializer);
}

#endif //SHAPEREALITY_ASSET_REFLECTION_H
