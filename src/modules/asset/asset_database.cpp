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
    AssetDatabase::AssetDatabase(BS::thread_pool& threadPool_,
                                 reflection::JsonSerializer& jsonSerializer_,
                                 ImportRegistry& importers_,
                                 fs::path inputDirectory_,
                                 fs::path loadDirectory_)
        : threadPool(threadPool_),
          jsonSerializer(jsonSerializer_),
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

    AssetDatabase::~AssetDatabase()
    {
        // for std::futures that were not created by std::async, we need to explicitly wait as the
        // destructor of a std::future does not wait.

        std::unique_lock<std::mutex> lock(importTasksMutex);
        std::vector<std::shared_future<void>> copied;
        copied.reserve(importTasks.size());
        for (auto& task: importTasks)
        {
            copied.emplace_back(task.second);
        }
        lock.unlock();

        for (auto& task: copied)
        {
            task.wait();
        }
    }

    Asset AssetDatabase::get(AssetId const& id)
    {
        return std::make_shared<AssetHandle>(id);
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

    bool AssetDatabase::valid(ImportResultCache const& importResultCache)
    {
        fs::path path = absolutePath(importResultCache.inputFilePath);
        return fs::last_write_time(path) != importResultCache.lastWriteTime;
    }

    void AssetDatabase::importFile(fs::path const& inputFile)
    {
        if (!fileExists(inputFile))
        {
            std::cout << "file does not exist" << std::endl;
            // error
            return;
        }

        if (!acceptsFile(inputFile))
        {
            std::cout << "file extension is not supported" << std::endl;
            // error
            return;
        }

        std::lock_guard<std::mutex> guard(importTasksMutex);

        if (taskIsRunning(inputFile))
        {
            std::cout << "task is already running" << std::endl;
            return;
        }

        if (importFromMemory(inputFile))
        {
            std::cout << "imported from memory" << std::endl;
            return;
        }

        if (importFromDisk(inputFile))
        {
            std::cout << "imported from disk" << std::endl;
            return;
        }

        startImportTask(inputFile);
    }

    bool AssetDatabase::importFromMemory(fs::path const& inputFile)
    {
        if (importResults.contains(inputFile))
        {
            ImportResultCache& entry = importResults.at(inputFile);
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

            // 2.2 convert to ImportResultCache
            auto result = jsonSerializer.fromJson<ImportResultCache>(data);

            if (valid(result))
            {
                // current file information is up-to-date!
                importResults.emplace(inputFile, result);
                std::cout << "imported from disk and emplaced in memory" << std::endl;
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
            fs::remove_all(cache); // make sure the load directory is set to a directory that does not contain
        }
    }

    bool AssetDatabase::taskIsRunning(fs::path const& inputFile)
    {
        return (importTasks.contains(inputFile) && importTasks.at(inputFile).valid());
    }

    void AssetDatabase::startImportTask(fs::path const& inputFile)
    {
        // we assume importTasksMutex is locked here (as this function only gets
        // called inside importFile, which has a lock_guard)

        std::cout << "start import task" << std::endl;

        std::shared_future<void> future = threadPool.submit_task([&, inputFile]() {

            std::vector<Asset> result = importers.importFile(absolutePath(inputFile));
            cache(inputFile, result);

            std::cout << "import task done" << std::endl;

            std::unique_lock<std::mutex> importTasksLock(importTasksMutex);
            importTasks.erase(inputFile);
            importTasksLock.unlock();
            std::cout << "import task erased" << std::endl;
        });
        importTasks.emplace(inputFile, std::move(future));
    }

    void AssetDatabase::cache(fs::path const& inputFile, std::vector<Asset> const& result)
    {
        if (result.empty())
        {
            return;
        }

        std::lock_guard<std::mutex> guard(importResultsMutex);

        // 1. store in memory
        importResults[inputFile] = createImportResultCache(inputFile, result);

        // 2. serialize to disk

        // 2.1 make sure the directory exists
        fs::path cacheDirectory = absoluteLoadPath(inputFile);
        if (!fs::exists(cacheDirectory))
        {
            fs::create_directories(cacheDirectory);
        }

        fs::path cacheFile = cacheDirectory / kImportResultFileName;

        // 2.2 write to file
        std::string serialized = jsonSerializer.toJsonString(result, kJsonIndentationAmount);
        std::ofstream serializedFile(cacheFile);
        serializedFile << serialized;
        serializedFile.close();

        // 3.
    }

    ImportResultCache
    AssetDatabase::createImportResultCache(fs::path const& inputFile, std::vector<Asset> const& result)
    {
        ImportResultCache cache{
            .inputFilePath = inputFile,
            .artifactPaths = {},
            .lastWriteTime = fs::last_write_time(absolutePath(inputFile))
        };
        cache.artifactPaths.reserve(result.size());
        for (auto& asset: result)
        {
            cache.artifactPaths.emplace_back(asset->assetId.artifactPath);
        }
        return cache;
    }
}