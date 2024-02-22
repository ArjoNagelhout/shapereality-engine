//
// Created by Arjo Nagelhout on 19/02/2024.
//

#include "asset_database.h"

#include <utility>
#include <iostream>

namespace asset
{
    // AssetId

    std::string AssetId::string() const
    {
        return "{ inputFilePath: " + inputFilePath.string() + ", artifactPath: " + artifactPath.string() + "}";
    }

    bool operator==(AssetId const& lhs, AssetId const& rhs)
    {
        return lhs.artifactPath == rhs.artifactPath &&
               lhs.inputFilePath == rhs.inputFilePath;
    }

    // AssetHandle

    AssetHandle::AssetHandle() : state(State::NotLoaded)
    {
    }

    AssetHandle::~AssetHandle() = default;

    // Import registry

    void ImportRegistry::emplace(ImportFunction&& function, std::vector<std::string> const& _extensions)
    {
        ImportFunction& f = functions.emplace_back(function);
        for (auto& extension : _extensions)
        {
            extensions.emplace(extension, f);
        }
    }

    bool ImportRegistry::contains(std::string const& extension)
    {
        return extensions.contains(extension);
    }

    // AssetDatabase

    AssetDatabase::AssetDatabase(ImportRegistry& _importers, fs::path _inputDirectory, fs::path _loadDirectory)
        : importers(_importers),
          inputDirectory(std::move(_inputDirectory)),
          loadDirectory(std::move(_loadDirectory))
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

    fs::path AssetDatabase::absolutePath(fs::path const& inputFile)
    {
        return inputDirectory / inputFile;
    }

    bool AssetDatabase::fileExists(fs::path const& inputFile)
    {
        fs::path path = absolutePath(inputFile);
        return fs::exists(path) && fs::is_regular_file(path);
    }

    bool AssetDatabase::acceptsFile(fs::path const& inputFile)
    {
        return fileExists(inputFile) && importers.contains(inputFile.extension());
    }

    bool AssetDatabase::fileChanged(InputFile const& inputFile)
    {
        fs::path path = absolutePath(inputFile.path);
        std::cout << fs::last_write_time(path) << std::endl;

        return true;
    }

    std::vector<AssetId> AssetDatabase::importFile(fs::path const& inputFile)
    {
        if (!acceptsFile(inputFile))
        {
            return {}; // error: invalid file
        }

        // import method:
        // 1. from memory (currently loaded / loading assets)
        if (inputFiles.contains(inputFile))
        {
            // check if current information is out of date
        }

        // 2. from cache located on disk in load directory

        // check if current information is out of date

        // 3. from input file

        fs::path const absolutePath = inputDirectory / inputFile;

        std::cout << absolutePath << std::endl;

        if (!fs::exists(absolutePath))
        {
            // error, input file does not exist
            return {};
        }

        return {};
    }
}