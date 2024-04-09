//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_REGISTER_ASSET_H
#define SHAPEREALITY_REGISTER_ASSET_H

#include <reflection/register.h>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace asset
{
    void pathFromJson(nlohmann::json const& in, std::filesystem::path* out);

    void pathToJson(std::filesystem::path* in, nlohmann::json& out);

    void fileTimeFromJson(nlohmann::json const& in, std::filesystem::file_time_type* out);

    void fileTimeToJson(std::filesystem::file_time_type* in, nlohmann::json& out);

    REGISTER_REFLECTION;
}

#endif //SHAPEREALITY_REGISTER_ASSET_H
