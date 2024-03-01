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

namespace fs = std::filesystem;

namespace asset
{
    struct AssetDatabase;

    using ImportFunction = std::function<std::vector<Asset>(AssetDatabase& assets,
                                                            std::filesystem::path const& inputFile)>;

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

        // we don't pass the import metadata to the import file function, this can be retrieved by the
        // import function itself.
        std::vector<Asset> importFile(AssetDatabase& assets, fs::path const& absolutePath);

    private:
        std::vector<ImportFunction> functions;
        std::unordered_map<std::string, size_t> extensions; // mapping from file extension to import functions
    };
}

#endif //SHAPEREALITY_IMPORT_REGISTRY_H
