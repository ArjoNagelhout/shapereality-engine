//
// Created by Arjo Nagelhout on 19/02/2024.
//

#include "asset_database.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <common/logger.h>
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
        common::log::info(
            "created asset database with: \n\tinput directory: {}\n\tload directory: {}",
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

    Asset AssetDatabase::get(AssetId const& id)
    {
        {
            std::lock_guard<std::mutex> guard(assetHandlesMutex);

            // check if asset handle has already been created
            if (assetHandles.contains(id))
            {
                return assetHandles.at(id).lock();
            }
        }

        // otherwise, start import
        importFile(id.inputFilePath);

        return std::make_shared<AssetHandle>(id);
    }

    std::filesystem::path AssetDatabase::absolutePath(std::filesystem::path const& inputFile) const
    {
        return parameters.inputDirectory / inputFile;
    }

    std::filesystem::path AssetDatabase::absoluteLoadPath(std::filesystem::path const& inputFile) const
    {
        std::string filtered = inputFile.generic_string();
        std::replace(filtered.begin(), filtered.end(), '.', '_');
        return parameters.loadDirectory / filtered;
    }

    bool AssetDatabase::fileExists(std::filesystem::path const& inputFile) const
    {
        std::filesystem::path path = absolutePath(inputFile);
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

    bool AssetDatabase::acceptsFile(std::filesystem::path const& inputFile)
    {
        return fileExists(inputFile) && context_.importers.contains(inputFile.extension());
    }

    bool AssetDatabase::valid(ImportResultCache const& importResultCache) const
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

        {
            std::lock_guard<std::mutex> guard(importTasksMutex);

            // 1. check if task is already running
            if (importTasks.contains(inputFile) && importTasks.at(inputFile).valid())
            {
                common::log::infoDebug("Import task for {} already running", absolutePath(inputFile).string());
                return;
            }

            // 2. otherwise, start import task
            common::log::infoDebug("Start import task for {}", absolutePath(inputFile).string());
            observers.invoke<&IAssetDatabaseObserver::onImportStarted>(inputFile);

            // 2.1 create task
            auto task = [&, inputFile]() {
                ImportResult result = context_.importers.importFile(*this, inputFile);
                if (result.error())
                {
                    common::log::error("Import failed for {} ({})", absolutePath(inputFile).string(), result.message());
                }

                if (result.success())
                {
                    std::lock_guard<std::mutex> assetHandlesLock(assetHandlesMutex);
                    // emplace asset handles
                    ImportResultData& data = result.get();
                    for (std::shared_ptr<AssetHandle>& artifact: data.artifacts)
                    {
                        // if the asset handle for the given AssetId already exists, do a switcheroo
                        // and give the asset data of the ImportResult to the existing handle
                        if (assetHandles.contains(artifact->id()))
                        {
                            Asset existingHandle = assetHandles.at(artifact->id()).lock();
                            existingHandle.swap(artifact);
                        }
                        else
                        {
                            // otherwise simply move it into the asset handles dictionary
                            assetHandles.emplace(artifact->id(), artifact);
                        }

                        common::log::infoDebug("emplaced {} into asset handles", artifact->id().artifactPath.string());
                    }
                }

                {
                    std::lock_guard<std::mutex> guard(importTasksMutex);
                    importTasks.erase(inputFile);
                }

                observers.invoke<&IAssetDatabaseObserver::onImportComplete>(inputFile, result);

                common::log::infoDebug("Import task done for {}", absolutePath(inputFile).string());
            };

            // 2.2 start and emplace task
            std::shared_future<void> future = threadPool.submit_task(std::move(task));
            importTasks.emplace(inputFile, std::move(future));
        }
    }

    AssetDatabaseContext const& AssetDatabase::context()
    {
        return context_;
    }
}