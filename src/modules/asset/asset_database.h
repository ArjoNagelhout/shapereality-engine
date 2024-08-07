//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_ASSET_DATABASE_H
#define SHAPEREALITY_ASSET_DATABASE_H

#include "asset_id.h"
#include "asset_handle.h"
#include "asset_type.h"
#include "import_registry.h"
#include "asset_type_registry.h"

#include <common/result.h>
#include <common/observers.h>
#include <common/thread_pool.h>
#include <reflection/reflection.h>

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <chrono>
#include <future>
#include <mutex>

namespace BS
{
    class thread_pool; // forward declaration
}

namespace asset
{
    // cache
    struct ImportResultCache
    {
        std::filesystem::path inputFilePath;
        std::vector<std::filesystem::path> artifacts;
        std::vector<std::filesystem::path> dependencies;
        std::filesystem::file_time_type lastWriteTime; // last write time of input file (not when it was imported)
    };

    class IAssetDatabaseObserver
    {
    public:
        virtual void onImportStarted(std::filesystem::path const& inputFile) = 0;

        virtual void onImportComplete(std::filesystem::path const& inputFile, ImportResult result) = 0;
    };

    // assets need a context in which they import the assets,
    // otherwise we would have to work with a singleton, or pass the graphics device to the import function
    // which is less elegant.
    struct AssetDatabaseContext
    {
        ImportRegistry& importers;
        AssetTypeRegistry& assetTypes;

        graphics::IDevice* device;
    };

    struct AssetDatabaseParameters
    {
        std::filesystem::path inputDirectory;
        std::filesystem::path loadDirectory;
        bool useImportCache;
    };

    /**
     * InputDirectory contains input files (e.g. some_input_file.gltf)
     *
     * LoadDirectory contains the results of importing the input files.
     *
     * Each imported results are put in directories with a serialized file import descriptor
     * this descriptor.
     */
    class AssetDatabase final
    {
    public:
        // file name for a cached input file
        constexpr static char const* kImportResultFileName = "import_result.json";
        constexpr static int kJsonIndentationAmount = 2;

        explicit AssetDatabase(
            AssetDatabaseParameters parameters,
            AssetDatabaseContext context,
            BS::thread_pool& threadPool = common::ThreadPool::shared());

        ~AssetDatabase();

        AssetDatabase(AssetDatabase const& rhs) = delete;

        // get an asset handle with the provided asset id
        [[nodiscard]] Asset get(AssetId const& id);

        // gets list of all assets that are produced as a result of importing inputFile
        void importFile(std::filesystem::path const& inputFile);

        // returns the absolute path of the provided input file
        [[nodiscard]] std::filesystem::path absolutePath(std::filesystem::path const& inputFile) const;

        // returns the absolute path of the load path that belongs to the provided input file
        // removes any dots from the file extension of the input file
        [[nodiscard]] std::filesystem::path absoluteLoadPath(std::filesystem::path const& inputFile) const;

        // returns whether the provided input file (relative path) exists
        // in the input directory
        [[nodiscard]] bool fileExists(std::filesystem::path const& inputFile) const;

        // returns whether an importer exists for the provided input file (relative path)
        [[nodiscard]] bool acceptsFile(std::filesystem::path const& inputFile);

        // returns whether the cache is up-to-date or whether we have to reimport
        [[nodiscard]] bool valid(ImportResultCache const& importResultCache) const;

        // observers for asset database events
        common::Observers<IAssetDatabaseObserver> observers;

        [[nodiscard]] AssetDatabaseContext const& context();

    private:
        AssetDatabaseContext context_;
        AssetDatabaseParameters parameters;
        BS::thread_pool& threadPool;

        std::unordered_map<AssetId, std::weak_ptr<AssetHandle>> assetHandles{};
        std::mutex assetHandlesMutex;

        // we use a shared future to enable copying in the destructor and waiting on them there,
        // while still enabling removing them from tasks on completion.
        std::unordered_map<std::filesystem::path, std::shared_future<void>> importTasks;
        std::mutex importTasksMutex;
    };
}

#endif //SHAPEREALITY_ASSET_DATABASE_H
