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
    AssetDatabase::AssetDatabase(
        AssetDatabaseParameters parameters_,
        AssetDatabaseContext context,
        BS::thread_pool& threadPool_)
        :
        parameters(std::move(parameters_)),
        context_(context),
        threadPool(threadPool_)
    {
        common::log::info("created asset database with: \n\tinput directory: {}\n\tload directory: {}",
                          parameters.inputDirectory.string(), parameters.loadDirectory.string());
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

    AssetBase AssetDatabase::get(AssetId const& id)
    {
        // check if asset handle has already been created
        if (assets.contains(id))
        {
            return assets.at(id).lock();
        }

        importFile(id.inputFilePath);

        return std::make_shared<AssetHandleBase>(id);
    }

    std::filesystem::path AssetDatabase::absolutePath(std::filesystem::path const& inputFile)
    {
        return parameters.inputDirectory / inputFile;
    }

    std::filesystem::path AssetDatabase::absoluteLoadPath(std::filesystem::path const& inputFile)
    {
        std::string filtered = inputFile.generic_string();
        std::replace(filtered.begin(), filtered.end(), '.', '_');
        return parameters.loadDirectory / filtered;
    }

    bool AssetDatabase::fileExists(std::filesystem::path const& inputFile)
    {
        std::filesystem::path path = absolutePath(inputFile);
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

    bool AssetDatabase::acceptsFile(std::filesystem::path const& inputFile)
    {
        return fileExists(inputFile) && context_.importers.contains(inputFile.extension());
    }

    bool AssetDatabase::valid(ImportResultCache const& importResultCache)
    {
        std::filesystem::path path = absolutePath(importResultCache.inputFilePath);
        return std::filesystem::last_write_time(path) != importResultCache.lastWriteTime;
    }

    void AssetDatabase::importFile(std::filesystem::path const& inputFile)
    {
        if (!fileExists(inputFile))
        {
            common::log::error("File does not exist: {}", absolutePath(inputFile).string());
            return;
        }

        if (!acceptsFile(inputFile))
        {
            common::log::error("Unsupported file format: {}", extension(inputFile));
            return;
        }

        std::lock_guard<std::mutex> guard(importTasksMutex);

        if (taskIsRunning(inputFile))
        {
            common::log::infoDebug("Import task for {} already running", absolutePath(inputFile).string());
            return;
        }

        ImportResultCache* cache;
        cache = getImportResultCacheFromMemory(inputFile);
        if (cache)
        {
            common::log::infoDebug("Got cached import result from memory for {}", absolutePath(inputFile).string());
            return;
        }

        cache = getImportResultCacheFromDisk(inputFile);
        if (cache)
        {
            return;
        }

        startImportTask(inputFile);
    }

    ImportResultCache* AssetDatabase::getImportResultCacheFromMemory(std::filesystem::path const& inputFile)
    {
        if (!parameters.useCache)
        {
            return nullptr;
        }

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

    ImportResultCache* AssetDatabase::getImportResultCacheFromDisk(std::filesystem::path const& inputFile)
    {
        if (!parameters.useCache)
        {
            return nullptr;
        }

        std::filesystem::path cachePath = absoluteLoadPath(inputFile) / kImportResultFileName;
        if (std::filesystem::exists(cachePath))
        {
            // 2.1 import json from file
            std::ifstream f(cachePath);
            nlohmann::json data = nlohmann::json::parse(f, nullptr, /*allow_exceptions*/ false, false);

            // 2.2 convert to ImportResultCache
            auto result = context_.reflection.json.fromJson<ImportResultCache>(data);

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

    void AssetDatabase::deleteImportResultFromCache(std::filesystem::path const& inputFile)
    {
        // delete from memory
        if (importResults.contains(inputFile))
        {
            importResults.erase(inputFile);
        }

        // delete local cache directory from load directory
        std::filesystem::path cachePath = absoluteLoadPath(inputFile);
        if (std::filesystem::exists(cachePath))
        {
            // make sure the load directory is set to a directory that does not contain any important files
            std::filesystem::remove_all(cachePath);
        }

        common::log::infoDebug("removed cache for {}", absolutePath(inputFile).string());
    }

    bool AssetDatabase::taskIsRunning(std::filesystem::path const& inputFile)
    {
        return (importTasks.contains(inputFile) && importTasks.at(inputFile).valid());
    }

    void AssetDatabase::startImportTask(std::filesystem::path const& inputFile)
    {
        // we assume importTasksMutex is locked here (as this function only gets
        // called inside importFile, which has a lock_guard)

        observers.invoke<&IAssetDatabaseObserver::onImportStarted>(inputFile);

        common::log::infoDebug("Start import task for {}", absolutePath(inputFile).string());

        std::shared_future<void> future = threadPool.submit_task([&, inputFile]() {
            ImportResult result = context_.importers.importFile(*this, inputFile);
            if (result.error())
            {
                common::log::error("Import failed for {} ({})", absolutePath(inputFile).string(), result.toString());
            }
            else
            {
                cacheImportResult(inputFile, result.get());
            }

            std::unique_lock<std::mutex> importTasksLock(importTasksMutex);
            importTasks.erase(inputFile);

            //observers.invoke<&IAssetDatabaseObserver::onImportComplete>(inputFile, result);

            importTasksLock.unlock();
            common::log::infoDebug("Import task cleared for {}", absolutePath(inputFile).string());
        });
        importTasks.emplace(inputFile, std::move(future));
    }

    void AssetDatabase::cacheImportResult(std::filesystem::path const& inputFile, ImportResultData const& result)
    {
        if (result.artifacts.empty())
        {
            return;
        }

        std::lock_guard<std::mutex> guard(importResultsMutex);

        // 1. store in memory
        ImportResultCache cache = createImportResultCache(inputFile, result);
        importResults[inputFile] = cache;

        // 2. serialize to disk

        // 2.1 make sure the directory exists
        std::filesystem::path cacheDirectory = absoluteLoadPath(inputFile);
        if (!std::filesystem::exists(cacheDirectory))
        {
            std::filesystem::create_directories(cacheDirectory);
        }

        std::filesystem::path cacheFile = cacheDirectory / kImportResultFileName;

        // 2.2 write to file
        std::string serialized = context_.reflection.json.toJsonString(cache, kJsonIndentationAmount);
        std::cout << serialized << std::endl;
        std::ofstream serializedFile(cacheFile);
        serializedFile << serialized;
        serializedFile.close();
    }

    ImportResultCache
    AssetDatabase::createImportResultCache(std::filesystem::path const& inputFile, ImportResultData const& result)
    {
        ImportResultCache cache{
            .inputFilePath = inputFile,
            .artifacts = {},
            .dependencies = {},
            .lastWriteTime = std::filesystem::last_write_time(absolutePath(inputFile))
        };
        cache.artifacts.reserve(result.artifacts.size());
        for (auto& asset: result.artifacts)
        {
            cache.artifacts.emplace_back(asset->id().artifactPath);
        }
        cache.dependencies = result.dependencies;
        return cache;
    }

    AssetDatabaseContext const& AssetDatabase::context()
    {
        return context_;
    }
}