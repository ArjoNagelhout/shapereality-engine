//
// Created by Arjo Nagelhout on 26/02/2024.
//

#ifndef SHAPEREALITY_IMPORT_REGISTRY_H
#define SHAPEREALITY_IMPORT_REGISTRY_H

#include <functional>
#include <unordered_map>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace asset
{
    using ImportFunction = std::function<void(fs::path const& absolutePath, std::function<void()> const& onComplete)>;

    /**
     *
     */
    class ImportRegistry
    {
    public:
        // add an import function for a set of file extensions
        void emplace(ImportFunction&& function, std::vector<std::string> const& extensions);

        [[nodiscard]] bool contains(std::string const& extension);

        // we don't pass the import metadata to the import file function, this can be retrieved by the
        // import function itself.
        void importFile(fs::path const& absolutePath, std::function<void()> const& onComplete);

    private:
        std::vector<ImportFunction> functions;
        std::unordered_map<std::string, ImportFunction&> extensions; // mapping from file extension to import functions
    };
}

#endif //SHAPEREALITY_IMPORT_REGISTRY_H
