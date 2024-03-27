//
// Created by Arjo Nagelhout on 26/02/2024.
//

#ifndef SHAPEREALITY_IMPORT_REGISTRY_H
#define SHAPEREALITY_IMPORT_REGISTRY_H

#include <functional>
#include <unordered_map>
#include <string>
#include <filesystem>

#include "asset_id.h"
#include "asset_handle.h"

#include <common/result.h>

namespace asset
{
    struct AssetDatabase;

    struct ImportResultData
    {
        std::vector<AssetBase> artifacts;
        std::vector<std::filesystem::path> dependencies; // input file paths for dependencies
    };

    using ImportResult = common::ValueResult<ImportResultData>;

    /**
     * ImportFunction is the type of function (be it a lambda or function pointer) that gets registered
     * inside the ImportRegistry.
     *
     * It takes two arguments:
     * assets: The AssetDatabase from which the input file should be imported
     * inputFile: The relative input file path
     *
     * This import function is subject to change depending on the wanted API for importing files.
     *
     * Warning: an ImportFunction is not allowed to wait for tasks enqueued to the shared thread pool,
     * as this could result in a thread starvation deadlock.
     */
    //@formatter:off
    using ImportFunction = std::function<ImportResult(AssetDatabase& assetDatabase, std::filesystem::path const& inputFile)>;
    //@formatter:on

    // extension without leading dot
    [[nodiscard]] std::string extension(std::filesystem::path const& path);

    [[nodiscard]] std::string removeLeadingDot(std::string string);

    /**
     *
     */
    class ImportRegistry
    {
    public:
        ~ImportRegistry();

        // add an import function for a set of file extensions
        void emplace(ImportFunction&& function, std::vector<std::string> const& extensions);

        [[nodiscard]] bool contains(std::string const& extension);

        /**
         * we don't pass the import metadata to the import file function, this can be retrieved by the
         * import function itself.
         *
         * not asynchronous, as that is handled by the AssetDatabase
         */
        ImportResult importFile(AssetDatabase& assetDatabase, std::filesystem::path const& absolutePath);

    private:
        std::vector<ImportFunction> functions;
        std::unordered_map<std::string, size_t> extensions; // mapping from file extension to import functions
    };
}

#endif //SHAPEREALITY_IMPORT_REGISTRY_H
