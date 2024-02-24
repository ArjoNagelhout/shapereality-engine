//
// Created by Arjo Nagelhout on 19/02/2024.
//

#include "asset_database.h"

#include <utility>
#include <iostream>

#include <common/log.h>

#include <fstream>
#include <nlohmann/json.hpp>

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
        for (auto& extension: _extensions)
        {
            extensions.emplace(extension, f);
        }
    }

    bool ImportRegistry::contains(std::string const& extension)
    {
        std::string e = extension;
        if (extension.starts_with('.'))
        {
            e = e.substr(1);
        }
        return extensions.contains(e);
    }

    // AssetDatabase

    AssetDatabase::AssetDatabase(reflection::JsonSerializer& serializer_,
                                 ImportRegistry& importers_,
                                 fs::path inputDirectory_,
                                 fs::path loadDirectory_)
        : serializer(serializer_),
          importers(importers_),
          inputDirectory(std::move(inputDirectory_)),
          loadDirectory(std::move(loadDirectory_))
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

    fs::path AssetDatabase::absoluteLoadPath(fs::path const& inputFile)
    {
        std::string filtered = inputFile.generic_string();
        std::replace(filtered.begin(), filtered.end(), '.', '_');
        return loadDirectory / filtered;
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
        return fs::last_write_time(path) != inputFile.lastWriteTime;
    }

    void AssetDatabase::importFile(fs::path const& inputFile, ImportCallback const& onComplete)
    {
        if (!fileExists(inputFile))
        {
            onComplete(ImportResult::makeError(ImportErrorCode::FileDoesNotExist,
                                               "File does not exist"));
            return;
        }

        if (!acceptsFile(inputFile))
        {
            onComplete(ImportResult::makeError(ImportErrorCode::FileNotAccepted, std::string(
                "no importers exist for the provided input file extension ") + inputFile.extension().generic_string()));
            return;
        }

        // import method:
        // 1. from memory (currently loaded / loading assets)
        if (inputFiles.contains(inputFile))
        {
            InputFile& entry = inputFiles[inputFile];
            if (!fileChanged(entry))
            {
                // current file information is up-to-date!
                onComplete(ImportResult::makeSuccess(&entry));
                return;
            }

            deleteFromCache(inputFile);
        }

        // 2. from cache located on disk in load directory
        fs::path cachedInputFile = absoluteLoadPath(inputFile) / kCachedInputFile;
        if (fs::exists(cachedInputFile))
        {
            // 1. import json from file
            std::ifstream f(cachedInputFile);
            nlohmann::json data = nlohmann::json::parse(f, nullptr, /*allow_exceptions*/ false, false);

            // 2. convert to InputFile
            auto result = serializer.fromJson<InputFile>(data);

            if (!fileChanged(result))
            {
                // current file information is up-to-date!
                inputFiles.emplace(inputFile, result);
            }
        }

        // 3. from input file

        auto [it, _] = inputFiles.emplace(inputFile, InputFile{});
        InputFile* result = &it->second;
        //onComplete(ImportResult::makeSuccess(result));
    }

    void AssetDatabase::deleteFromCache(fs::path const& inputFile)
    {
        // delete from memory
        if (inputFiles.contains(inputFile))
        {
            inputFiles.erase(inputFile);
        }

        // delete local cache directory from load directory
        fs::path cache = absoluteLoadPath(inputFile);
        std::cout << cache << std::endl;
        if (fs::exists(cache))
        {
            fs::remove(cache);
        }

        // send input file deleted event? So that all loaded assets can be unloaded?

    }
}