//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_ASSET_DATABASE_H
#define SHAPEREALITY_ASSET_DATABASE_H

#include <filesystem>

namespace fs = std::filesystem;

namespace asset
{
    class AssetHandle final
    {
    public:
        enum class State
        {
            NotLoaded,
            Loading,
            Success,
            Error
        };

        explicit AssetHandle();

        ~AssetHandle();

    private:
        std::string errorMessage;
        State state;
    };

    struct AssetId final
    {
        fs::path inputFilePath; // path relative to a source directory
        fs::path artifactPath;
    };

    /**
     * InputDirectory contains input files (e.g. mesh.fbx)
     *
     */
    class AssetDatabase final
    {
    public:
        explicit AssetDatabase(fs::path inputDirectory, fs::path loadDirectory);

        ~AssetDatabase();

        [[nodiscard]] AssetHandle get(AssetId const& id);

    private:
        fs::path const inputDirectory;
        fs::path const loadDirectory;
    };
}

#endif //SHAPEREALITY_ASSET_DATABASE_H
