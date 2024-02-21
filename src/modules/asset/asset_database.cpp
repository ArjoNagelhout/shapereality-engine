//
// Created by Arjo Nagelhout on 19/02/2024.
//

#include "asset_database.h"

#include <utility>
#include <iostream>

namespace asset
{
    std::string AssetId::string() const
    {
        return "{ inputFilePath: " + inputFilePath.string() + ", artifactPath: " + artifactPath.string() + "}";
    }

    bool operator==(AssetId const& lhs, AssetId const& rhs)
    {
        return lhs.artifactPath == rhs.artifactPath &&
               lhs.inputFilePath == rhs.inputFilePath;
    }

    AssetHandle::AssetHandle() : state(State::NotLoaded)
    {
    }

    AssetHandle::~AssetHandle() = default;

    AssetDatabase::AssetDatabase(fs::path _inputDirectory, fs::path _loadDirectory)
        : inputDirectory(std::move(_inputDirectory)), loadDirectory(std::move(_loadDirectory))
    {
        std::cout << "created asset database with \n\tinput directory: "
                  << inputDirectory
                  << "\n\tload directory: "
                  << loadDirectory
                  << std::endl;
    }

    AssetDatabase::~AssetDatabase() = default;

    fs::path getArtifactPath(AssetId const& id)
    {
        return {};
    }

    void loadArtifact(AssetId const& id, AssetHandle& handle)
    {
        fs::path artifactPath = getArtifactPath(id);
    }

    void importInputFile(AssetId const& id, AssetHandle& handle)
    {
        // on complete:
        loadArtifact(id, handle);
    }

    std::shared_ptr<AssetHandle> AssetDatabase::get(AssetId const& id)
    {
        // if asset handle is already created, return that one.
        // AssetDatabase should have a map of ids to asset handles
        if (assets.contains(id))
        {
            std::weak_ptr<AssetHandle>& asset = assets[id];
            if (!asset.expired())
            {
                return asset.lock();
            }
        }

        fs::path path = getArtifactPath(id);
        AssetHandle handle;
        if (fs::exists(path))
        {
            // load artifact
            loadArtifact(id, handle);
        }
        else
        {
            // import input file
            importInputFile(id, handle);
        }

        return std::make_shared<AssetHandle>(std::move(handle));
    }

    std::vector<AssetId> AssetDatabase::importFile(fs::path const& inputFile)
    {
        fs::path const absolutePath = inputDirectory / inputFile;

        if (fs::exists(absolutePath))
        {

        }

        // first check if the input file was already imported, and whether
        // it is up-to-date.
        return {};
    }
}