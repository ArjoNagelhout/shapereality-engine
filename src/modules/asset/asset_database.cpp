//
// Created by Arjo Nagelhout on 19/02/2024.
//

#include "asset_database.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <common/logger.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <BS_thread_pool.hpp>

namespace asset
{
    AssetDatabase::AssetDatabase(BS::thread_pool& threadPool_,
                                 reflection::JsonSerializer& jsonSerializer_,
                                 ImportRegistry& importers_,
                                 fs::path inputDirectory_,
                                 fs::path loadDirectory_,
                                 bool useCache_)
        : threadPool(threadPool_),
          jsonSerializer(jsonSerializer_),
          importers(importers_),
          inputDirectory(std::move(inputDirectory_)),
          loadDirectory(std::move(loadDirectory_)),
          useCache(useCache_)
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
        std::vector<std::shared_future<void>> copiedTasks;
        copiedTasks.reserve(importTasks.size());
        for (auto& task: importTasks)
        {
            copiedTasks.emplace_back(task.second);
        }
        lock.unlock();

        for (auto& task: copiedTasks)
        {
            task.wait();
        }
    }

    Asset AssetDatabase::get(AssetId const& id)
    {
        // check if asset handle has already been created
        if (assets.contains(id))
        {
            return assets.at(id).lock();
        }

        importFile(id.inputFilePath);

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
            common::log(std::string("File does not exist: ") + absolutePath(inputFile).string(), common::Severity_Error, common::Verbosity::Release);
            return;
        }

        if (!acceptsFile(inputFile))
        {
            common::log(std::string("Unsupported file format: ") + extension(inputFile), common::Severity_Error, common::Verbosity::Release);
            return;
        }

        std::lock_guard<std::mutex> guard(importTasksMutex);

        if (taskIsRunning(inputFile))
        {
            common::log(std::string("Import task for ") + absolutePath(inputFile).string() + " already running", common::Severity_Info, common::Verbosity::Debug);
            return;
        }

        ImportResultCache* cache;
        cache = getImportResultCacheFromMemory(inputFile);
        if (cache)
        {
            common::log(std::string("Got cached import result from memory for ") + absolutePath(inputFile).string(), common::Severity_Info, common::Verbosity::Debug);

            return;
        }

        cache = getImportResultCacheFromDisk(inputFile);
        if (cache)
        {
            return;
        }

        startImportTask(inputFile);
    }

    ImportResultCache* AssetDatabase::getImportResultCacheFromMemory(fs::path const& inputFile)
    {
        if (importResults.contains(inputFile))
        {
            ImportResultCache& entry = importResults.at(inputFile);
            if (valid(entry))
            {
                // current file information is up-to-date!
                return &entry;
            }
            else
            {
                // file was changed, so delete cache and go to 2.
                deleteImportResultFromCache(inputFile);
            }
        }
        return nullptr;
    }

    ImportResultCache* AssetDatabase::getImportResultCacheFromDisk(fs::path const& inputFile)
    {
        fs::path cachePath = absoluteLoadPath(inputFile) / kImportResultFileName;
        if (fs::exists(cachePath))
        {
            // 2.1 import json from file
            std::ifstream f(cachePath);
            nlohmann::json data = nlohmann::json::parse(f, nullptr, /*allow_exceptions*/ false, false);

            // 2.2 convert to ImportResultCache
            auto result = jsonSerializer.fromJson<ImportResultCache>(data);

            if (valid(result))
            {
                // current file information is up-to-date!
                auto emplaceResult = importResults.emplace(inputFile, result);
                return &emplaceResult.first->second;
            }
            else
            {
                // file was changed, so delete cache and go to 3.
                deleteImportResultFromCache(inputFile);
            }
        }
        return nullptr;
    }

    void AssetDatabase::deleteImportResultFromCache(fs::path const& inputFile)
    {
        // delete from memory
        if (importResults.contains(inputFile))
        {
            importResults.erase(inputFile);
        }

        // delete local cache directory from load directory
        fs::path cachePath = absoluteLoadPath(inputFile);
        if (fs::exists(cachePath))
        {
            // make sure the load directory is set to a directory that does not contain any important files
            fs::remove_all(cachePath);
        }

        common::log(std::string("removed cache for ") + absolutePath(inputFile).string(), common::Severity_Info, common::Verbosity::Debug);
    }

    bool AssetDatabase::taskIsRunning(fs::path const& inputFile)
    {
        return (importTasks.contains(inputFile) && importTasks.at(inputFile).valid());
    }

    void AssetDatabase::startImportTask(fs::path const& inputFile)
    {
        // we assume importTasksMutex is locked here (as this function only gets
        // called inside importFile, which has a lock_guard)

        observers.invoke<&IAssetDatabaseObserver::onImportStarted>(inputFile);

        std::cout << "start import task" << std::endl;

        std::shared_future<void> future = threadPool.submit_task([&, inputFile]() {

            ImportResult result = importers.importFile(*this, inputFile);
            if (result.error())
            {

            }
            else
            {
                std::cout << "import task done" << std::endl;
                cache(inputFile, result.get());
            }

            std::unique_lock<std::mutex> importTasksLock(importTasksMutex);
            importTasks.erase(inputFile);

            observers.invoke<&IAssetDatabaseObserver::onImportComplete>();

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
        ImportResultCache cache = createImportResultCache(inputFile, result);
        importResults[inputFile] = cache;

        // 2. serialize to disk

        // 2.1 make sure the directory exists
        fs::path cacheDirectory = absoluteLoadPath(inputFile);
        if (!fs::exists(cacheDirectory))
        {
            fs::create_directories(cacheDirectory);
        }

        fs::path cacheFile = cacheDirectory / kImportResultFileName;

        // 2.2 write to file
        std::string serialized = jsonSerializer.toJsonString(cache, kJsonIndentationAmount);
        std::cout << serialized << std::endl;
        std::ofstream serializedFile(cacheFile);
        serializedFile << serialized;
        serializedFile.close();
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
            cache.artifactPaths.emplace_back(asset->id().artifactPath);
        }
        return cache;
    }
}