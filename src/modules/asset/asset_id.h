//
// Created by Arjo Nagelhout on 26/02/2024.
//

#ifndef SHAPEREALITY_ASSET_ID_H
#define SHAPEREALITY_ASSET_ID_H

#include <filesystem>

namespace fs = std::filesystem;

namespace asset
{
    struct AssetId final
    {
        fs::path inputFilePath; // path relative to a source directory
        fs::path artifactPath; // path relative to the output file

        [[nodiscard]] std::string string() const;
    };

    [[nodiscard]] bool operator==(AssetId const& lhs, AssetId const& rhs);
}

// the c++ standard library shipped with XCode does not have the correct hash template
// specialization, so we provide one here.
template<>
struct std::hash<fs::path>
{
    [[nodiscard]] size_t operator()(fs::path const& path) const
    {
        return std::hash<std::string>{}(path.generic_string());
    }
};

template<>
struct std::hash<asset::AssetId>
{
    [[nodiscard]] size_t operator()(asset::AssetId const& id) const
    {
        size_t inputFilePathHash = std::hash<fs::path>{}(id.inputFilePath);
        size_t artifactPathHash = std::hash<fs::path>{}(id.artifactPath);
        return inputFilePathHash ^ artifactPathHash;
    }
};

#endif //SHAPEREALITY_ASSET_ID_H
