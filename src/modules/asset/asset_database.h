//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_ASSET_DATABASE_H
#define SHAPEREALITY_ASSET_DATABASE_H

#include "asset_id.h"
#include "import_registry.h"

#include <common/result.h>
#include <reflection/serialize/json.h>

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <chrono>
#include <future>
#include <mutex>

namespace fs = std::filesystem;

using namespace common;

namespace BS
{
    class thread_pool; // forward declaration
}

namespace asset
{
    class AssetHandle final
    {
    public:
        enum class State
        {
            NotLoaded,
            Loading,
            Success,
            Error
        };

        explicit AssetHandle();

        ~AssetHandle();

    private:
        std::string errorMessage; // e.g. input file does not contain referenced artifact
        State state;
    };

    struct ImportResult
    {
        fs::path inputFilePath;
        std::vector<fs::path> artifactPaths;
        fs::file_time_type lastWriteTime; // last write time of input file (not when it was imported)
    };

    struct ImportTask
    {
        std::future<void> future;

        explicit ImportTask(std::future<void>&& future_);
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
                               reflection::JsonSerializer& serializer,
                               ImportRegistry& importers,
                               fs::path inputDirectory,
                               fs::path loadDirectory);

        ~AssetDatabase();

        //
        [[nodiscard]] std::shared_ptr<AssetHandle> get(AssetId const& id);

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
        [[nodiscard]] bool valid(ImportResult const& importResultCache);

        //
        void importFile(fs::path const& inputFile);

    private:
        BS::thread_pool& threadPool; // thread pool for submitting import tasks, could be made into a singleton
        reflection::JsonSerializer& serializer; // serialize to and from json, could be made into a singleton
        ImportRegistry& importers; // registry containing an import function for each registered file extension

        fs::path const inputDirectory; //
        fs::path const loadDirectory; // directory containing engine native files and input file cache descriptor

        std::unordered_map<AssetId, std::weak_ptr<AssetHandle>> assets; // assets that are loaded or being loaded

        std::unordered_map<fs::path, ImportResult> importResults;
        std::unordered_map<fs::path, ImportTask> importTasks;
        std::mutex importTasksMutex; // locked when updating tasks and results

        // returns whether importing from memory was successful
        [[nodiscard]] bool importFromMemory(fs::path const& inputFile);

        // returns whether importing from disk was successful
        [[nodiscard]] bool importFromDisk(fs::path const& inputFile);

        // removes the input file from memory and from disk if it exists there
        void deleteFromCache(fs::path const& inputFile);

        // starts an import task for the provided input file, we assume that
        // no other import task has been created for this input file.
        // the task gets submitted to the task queue of the thread pool
        void startImportTask(fs::path const& inputFile);
    };
}

#endif //SHAPEREALITY_ASSET_DATABASE_H
