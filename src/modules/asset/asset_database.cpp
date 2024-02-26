//
// Created by Arjo Nagelhout on 19/02/2024.
//

#include "asset_database.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <common/log.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <BS_thread_pool.hpp>

namespace asset
{
    // AssetHandle

    AssetHandle::AssetHandle() : state(State::NotLoaded)
    {
    }

    AssetHandle::~AssetHandle() = default;

    ImportTask::ImportTask(std::future<void>&& future_) : future(std::move(future_)) {}

    AssetDatabase::AssetDatabase(BS::thread_pool& threadPool_,
                                 reflection::JsonSerializer& serializer_,
                                 ImportRegistry& importers_,
                                 fs::path inputDirectory_,
                                 fs::path loadDirectory_)
        : threadPool(threadPool_),
          serializer(serializer_),
          importers(importers_),
          inputDirectory(std::move(inputDirectory_)),
          loadDirectory(std::move(loadDirectory_))
    {
        std::cout << "created asset database with: \n\tinput directory: "
                  << inputDirectory
                  << "\n\tload directory: "
                  << loadDirectory
                  << std::endl;
    }

    AssetDatabase::~AssetDatabase() = default;

    std::shared_ptr<AssetHandle> AssetDatabase::get(AssetId const& id)
    {
        return std::make_shared<AssetHandle>();
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

    bool AssetDatabase::valid(ImportResult const& importResult)
    {
        fs::path path = absolutePath(importResult.inputFilePath);
        return fs::last_write_time(path) != importResult.lastWriteTime;
    }

    void AssetDatabase::importFile(fs::path const& inputFile)
    {
        if (!fileExists(inputFile))
        {
            // error
            return;
        }

        if (!acceptsFile(inputFile))
        {
            // error
            return;
        }

        std::lock_guard<std::mutex> guard(importTasksMutex);

        if (importTasks.contains(inputFile))
        {
            return; // already running import task
        }

        if (importFromMemory(inputFile))
        {
            return;
        }

        if (importFromDisk(inputFile))
        {
            return;
        }

        startImportTask(inputFile);
    }

    bool AssetDatabase::importFromMemory(fs::path const& inputFile)
    {
        if (importResults.contains(inputFile))
        {
            ImportResult& entry = importResults.at(inputFile);
            if (valid(entry))
            {
                // current file information is up-to-date!
                //onComplete(ImportResult::makeSuccess(&entry));
                return true;
            }
            else
            {
                // file was changed, so delete cache and go to 2.
                deleteFromCache(inputFile);
            }
        }
        return false;
    }

    bool AssetDatabase::importFromDisk(fs::path const& inputFile)
    {
        fs::path cache = absoluteLoadPath(inputFile) / kImportResultFileName;
        if (fs::exists(cache))
        {
            // 2.1 import json from file
            std::ifstream f(cache);
            nlohmann::json data = nlohmann::json::parse(f, nullptr, /*allow_exceptions*/ false, false);

            // 2.2 convert to ImportResult
            auto result = serializer.fromJson<ImportResult>(data);

            if (valid(result))
            {
                // current file information is up-to-date!
                importResults.emplace(inputFile, result);
                return true;
            }
            else
            {
                // file was changed, so delete cache and go to 3.
                deleteFromCache(inputFile);
            }
        }
        return false;
    }

    void AssetDatabase::deleteFromCache(fs::path const& inputFile)
    {
        // delete from memory
        if (importResults.contains(inputFile))
        {
            importResults.erase(inputFile);
        }

        // delete local cache directory from load directory
        fs::path cache = absoluteLoadPath(inputFile);
        std::cout << cache << std::endl;
        if (fs::exists(cache))
        {
            fs::remove_all(cache); // warning: removes the cache directory recursively, without warning!
        }

        // send input file deleted event? So that all loaded assets can be unloaded?
    }

    void AssetDatabase::startImportTask(fs::path const& inputFile)
    {

    }
}