//
// Created by Arjo Nagelhout on 30/01/2024.
//

#include "assets_source_directory.h"

#include <utility>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

namespace assets
{
    void createMetafile()
    {
        // should call serialize on an asset
    }

    SourceAssetsDirectory::SourceAssetsDirectory(fs::path _sourceDirectory,
                                                 fs::path _cacheDirectory)
        : sourceDirectory(std::move(_sourceDirectory)), cacheDirectory(std::move(_cacheDirectory))
    {
        populateSourceAssets();
        createMetaFiles();
    }

    SourceAssetsDirectory::~SourceAssetsDirectory() = default;

    void SourceAssetsDirectory::populateSourceAssets()
    {
        sourceAssets.clear();

        for (auto const& entry: fs::recursive_directory_iterator(sourceDirectory))
        {
            if (!entry.is_regular_file())
            {
                continue;
            }

            fs::path const& path = entry.path();
            fs::path const& relativePath = fs::relative(path, sourceDirectory);

            // filter files that match the .gitignore like file
            if (blacklist.contains(path.filename().string()))
            {
                continue;
            }

            // filter meta files
            if (path.extension() == kMetaFileExtension)
            {
                continue;
            }

            // get relative path
            std::cout << relativePath << std::endl;

            // this is going to be the identifier inside the map
            // we only store asset files, no directories
            sourceAssets[relativePath] = SourceAsset{};
        }
    }

    void SourceAssetsDirectory::createMetaFiles()
    {
        for (auto& entry: sourceAssets)
        {
            std::string const relativePath = entry.first;
            fs::path absolutePath = sourceDirectory / relativePath;

            // get meta path
            fs::path metaPath = absolutePath;
            metaPath.concat(kMetaFileExtension);

            // create meta file if it does not exist yet

            std::ofstream out(metaPath.string());
        }
    }
}