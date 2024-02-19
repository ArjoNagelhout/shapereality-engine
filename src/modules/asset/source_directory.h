//
// Created by Arjo Nagelhout on 30/01/2024.
//

#ifndef SHAPEREALITY_SOURCE_DIRECTORY_H
#define SHAPEREALITY_SOURCE_DIRECTORY_H

#include <filesystem>
#include <string>
#include <unordered_set>

/**
 * @namespace asset
 * @brief classes related to importing and serializing assets on both compile time and runtime
 */
namespace asset
{
    constexpr char const* kMetaFileExtension = ".meta"; // dot is important

    struct SourceAsset
    {

    };

    // the assets source directory watches a directory and
    // creates an index from its contents.
    //
    // each file inside the source directory will get a .meta file
    // added next to them in the containing directory.
    //
    // this .meta file contains metadata on how the asset should be imported
    //
    // the imported assets get cached in the cacheDirectory.
    //
    // only one AssetsSourceDirectory should be active on a given source or cache directory.
    class SourceDirectory
    {
    public:
        explicit SourceDirectory(std::filesystem::path sourceDirectory, std::filesystem::path cacheDirectory);

        ~SourceDirectory();

    private:
        // this is the place where sources are stored
        std::filesystem::path sourceDirectory;

        // where the imported assets are stored
        std::filesystem::path cacheDirectory;

        //
        std::unordered_map<std::string, SourceAsset> sourceAssets;

        //
        std::unordered_set<std::string> blacklist{
            ".DS_Store"
        };

        //
        void loadSourceAssets();

        //
        void createMetaFiles();
    };
}

#endif //SHAPEREALITY_SOURCE_DIRECTORY_H
