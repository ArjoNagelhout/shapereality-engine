//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_ASSET_DATABASE_H
#define SHAPEREALITY_ASSET_DATABASE_H

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <chrono>

#include <common/result.h>

namespace fs = std::filesystem;

using namespace common;

namespace asset
{
    struct AssetId final
    {
        fs::path inputFilePath; // path relative to a source directory
        fs::path artifactPath; // path relative to the output file

        [[nodiscard]] std::string string() const;
    };

    [[nodiscard]] bool operator==(AssetId const& lhs, AssetId const& rhs);
}

// the c++ standard library shipped with XCode does not have the correct hash template
// specialization, so we provide one here.
template<>
struct std::hash<fs::path>
{
    [[nodiscard]] size_t operator()(fs::path const& path) const
    {
        size_t hash = std::hash<std::string>{}(path.generic_string());
        std::cout << hash << std::endl;
        return hash;
    }
};

template<>
struct std::hash<asset::AssetId>
{
    [[nodiscard]] size_t operator()(asset::AssetId const& id) const
    {
        size_t inputFilePathHash = std::hash<fs::path>{}(id.inputFilePath);
        size_t artifactPathHash = std::hash<fs::path>{}(id.artifactPath);
        size_t hash = inputFilePathHash ^ artifactPathHash;
        std::cout << hash << std::endl;
        return hash;
    }
};

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

        explicit AssetDatabase(ImportRegistry& importers, fs::path inputDirectory, fs::path loadDirectory);

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

    private:
        ImportRegistry& importers;

        // directories
        fs::path const inputDirectory;
        fs::path const loadDirectory;

        // assets that are loaded or being loaded
        std::unordered_map<AssetId, std::weak_ptr<AssetHandle>> assets;

        // imported input files (path keys are relative to the input directory)
        std::unordered_map<fs::path, InputFile> inputFiles;
    };
}

#endif //SHAPEREALITY_ASSET_DATABASE_H
