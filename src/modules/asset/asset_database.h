//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_ASSET_DATABASE_H
#define SHAPEREALITY_ASSET_DATABASE_H

#include "asset_id.h"

#include <common/result.h>
#include <reflection/serialize/json.h>

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <chrono>

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

    using ImportFunction = std::function<void(std::function<void()> const& onComplete)>;

    class ImportRegistry
    {
    public:
        // add an import function for a set of file extensions
        void emplace(ImportFunction&& function, std::vector<std::string> const& extensions);

        [[nodiscard]] bool contains(std::string const& extension);

    private:
        std::vector<ImportFunction> functions;
        std::unordered_map<std::string, ImportFunction&> extensions; // mapping from file extension to import functions
    };

    // serializable input file information, such as which artifacts it produces and
    // when the file was last written to / modified
    struct InputFile
    {
        fs::path path;
        std::vector<AssetId> artifacts;
        fs::file_time_type lastWriteTime; // last write time of input file (not when it was imported)
    };

    enum class ImportErrorCode : size_t
    {
        None = 0,
        FileDoesNotExist,
        FileNotAccepted
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
        constexpr static char const* kCachedInputFile = "input_file.json";
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

        // returns whether the input file was changed through comparing modified
        // dates or content hashes, given the provided input file data
        [[nodiscard]] bool fileChanged(InputFile const& inputFile);

        using ImportResult = Result<InputFile*>;
        using ImportCallback = std::function<void(ImportResult)>;

        // asynchronous function that calls the callback on complete
        void importFile(fs::path const& inputFile, std::function<void(Result<InputFile*>)> const& onComplete);

        // removes the input file from memory and from disk if it exists there
        void deleteFromCache(fs::path const& inputFile);

    private:
        BS::thread_pool& threadPool; // thread pool for submitting import tasks, could be made into a singleton
        reflection::JsonSerializer& serializer; // serialize to and from json, could be made into a singleton

        ImportRegistry& importers; // registry containing an import function for each registered file extension

        fs::path const inputDirectory; //
        fs::path const loadDirectory; // directory containing engine native files and input file cache descriptor

        // assets that are loaded or being loaded
        std::unordered_map<AssetId, std::weak_ptr<AssetHandle>> assets;
        std::unordered_map<fs::path, InputFile> inputFiles; // imported input files (path keys are relative to the input directory)
    };
}

#endif //SHAPEREALITY_ASSET_DATABASE_H
