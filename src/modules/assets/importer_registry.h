//
// Created by Arjo Nagelhout on 18/02/2024.
//

#ifndef SHAPEREALITY_IMPORTER_REGISTRY_H
#define SHAPEREALITY_IMPORTER_REGISTRY_H

#include <string>

namespace assets
{
    // todo: thumbnail generation

    using ImportFunction = std::function<void(std::string const& sourceFilePath, std::string const& targetFilePath)>;

    // an AssetRegistry contains a mapping of asset types to file formats and their
    // corresponding importers
    class ImporterRegistry
    {
    public:
        explicit ImporterRegistry();
        ~ImporterRegistry();

        void emplace(std::vector<std::string> const& fileExtensions, ImportFunction&& importFunction);

        // should automatically look for a meta file that accompanies the source file path
        void importFile(std::string const& sourceFilePath, std::string const& targetFilePath);

    private:
        std::vector<ImportFunction> functions;

    };
}

#endif //SHAPEREALITY_IMPORTER_REGISTRY_H
