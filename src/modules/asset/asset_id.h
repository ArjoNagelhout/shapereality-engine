//
// Created by Arjo Nagelhout on 26/02/2024.
//

#ifndef SHAPEREALITY_ASSET_ID_H
#define SHAPEREALITY_ASSET_ID_H

#include <filesystem>

namespace asset
{
    struct AssetId final
    {
        std::filesystem::path inputFilePath; // path relative to a source directory
        std::filesystem::path artifactPath; // path relative to the output file

        [[nodiscard]] std::string string() const;
    };

    [[nodiscard]] bool operator==(AssetId const& lhs, AssetId const& rhs);
}

// the c++ standard library shipped with XCode does not have the correct hash template
// specialization, so we provide one here.
template<>
struct std::hash<std::filesystem::path>
{
    [[nodiscard]] size_t operator()(std::filesystem::path const& path) const
    {
        return std::hash<std::string>{}(path.generic_string());
    }
};

template<>
struct std::hash<asset::AssetId>
{
    [[nodiscard]] size_t operator()(asset::AssetId const& id) const
    {
        size_t inputFilePathHash = std::hash<std::filesystem::path>{}(id.inputFilePath);
        size_t artifactPathHash = std::hash<std::filesystem::path>{}(id.artifactPath);
        return inputFilePathHash ^ artifactPathHash;
    }
};

#endif //SHAPEREALITY_ASSET_ID_H
