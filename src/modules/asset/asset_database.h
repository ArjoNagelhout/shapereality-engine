//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_ASSET_DATABASE_H
#define SHAPEREALITY_ASSET_DATABASE_H

#include "asset_id.h"
#include "asset_handle.h"
#include "import_registry.h"

#include <common/result.h>
#include <reflection/serialize/json.h>
#include <reflection/type_info.h>

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <chrono>
#include <future>
#include <mutex>

namespace fs = std::filesystem;

namespace BS
{
    class thread_pool; // forward declaration
}

namespace asset
{
    // cache
    struct ImportResultCache
    {
        fs::path inputFilePath;
        std::vector<fs::path> artifactPaths;
        std::filesystem::file_time_type lastWriteTime; // last write time of input file (not when it was imported)
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

        explicit AssetDatabase(BS::thread_pool& threadPool,
                               reflection::JsonSerializer& jsonSerializer,
                               ImportRegistry& importers,
                               fs::path inputDirectory,
                               fs::path loadDirectory);

        ~AssetDatabase();

        // get an asset with the provided asset id
        [[nodiscard]] Asset get(AssetId const& id);

        // gets list of all assets that are produced as a result of importing inputFile
        void importFile(fs::path const& inputFile);

        // returns the absolute path of the provided input file
        [[nodiscard]] fs::path absolutePath(fs::path const& inputFile);

        // returns the absolute path of the load path that belongs to the provided input file
        // removes any dots from the file extension of the input file
        [[nodiscard]] fs::path absoluteLoadPath(fs::path const& inputFile);

        // returns whether the provided input file (relative path) exists
        // in the input directory
        [[nodiscard]] bool fileExists(fs::path const& inputFile);

        // returns whether an importer exists for the provided input file (relative path)
        [[nodiscard]] bool acceptsFile(fs::path const& inputFile);

        // returns whether the cache is up-to-date or whether we have to reimport
        [[nodiscard]] bool valid(ImportResultCache const& importResultCache);

    private:
        BS::thread_pool& threadPool;
        reflection::JsonSerializer& jsonSerializer;
        ImportRegistry& importers;

        fs::path const inputDirectory;
        fs::path const loadDirectory;

        std::unordered_map<AssetId, std::weak_ptr<AssetHandle>> assets{};
        std::unordered_map<fs::path, ImportResultCache> importResults;
        std::mutex importResultsMutex;

        // we use a shared future to enable copying in the destructor and waiting on them there,
        // while still enabling removing them from tasks on completion.
        std::unordered_map<fs::path, std::shared_future<void>> importTasks;
        std::mutex importTasksMutex;

        // returns whether importing from memory was successful
        [[nodiscard]] bool importFromMemory(fs::path const& inputFile);

        // returns whether importing from disk was successful
        [[nodiscard]] bool importFromDisk(fs::path const& inputFile);

        // removes the input file from memory and from disk if it exists there
        void deleteFromCache(fs::path const& inputFile);

        // returns whether the task is currently running, and if it still exists as an invalid future in the
        // importTasks unordered_map, it gets removed
        [[nodiscard]] bool taskIsRunning(fs::path const& inputFile);

        // starts an import task for the provided input file, we assume that
        // no other import task has been created for this input file.
        // the task gets submitted to the task queue of the thread pool
        void startImportTask(fs::path const& inputFile);

        // store import result in memory and store in disk
        void cache(fs::path const& inputFile, std::vector<Asset> const& result);

        [[nodiscard]] ImportResultCache
        createImportResultCache(fs::path const& inputFile, std::vector<Asset> const& result);
    };
}

#endif //SHAPEREALITY_ASSET_DATABASE_H
