//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_ASSET_DATABASE_H
#define SHAPEREALITY_ASSET_DATABASE_H

#include <filesystem>
#include <unordered_map>
#include <vector>

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

template<>
struct std::hash<asset::AssetId>
{
    [[nodiscard]] size_t operator()(asset::AssetId const& id) const
    {
        return 0;
    }
};

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
        std::string errorMessage; // e.g. input file does not contain referenced artifact
        State state;
    };

    /**
     * InputDirectory contains input files (e.g. some_input_file.gltf)
     */
    class AssetDatabase final
    {
    public:
        explicit AssetDatabase(fs::path inputDirectory, fs::path loadDirectory);

        ~AssetDatabase();

        [[nodiscard]] std::shared_ptr<AssetHandle> get(AssetId const& id);

        [[nodiscard]] std::vector<AssetId> importFile(fs::path const& inputFile);

    private:
        fs::path const inputDirectory;
        fs::path const loadDirectory;
        std::unordered_map<AssetId, std::weak_ptr<AssetHandle>> assets; // assets that are loaded or being loaded

    };
}

#endif //SHAPEREALITY_ASSET_DATABASE_H
