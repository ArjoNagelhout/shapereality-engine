//
// Created by Arjo Nagelhout on 18/02/2024.
//

#ifndef SHAPEREALITY_IMPORT_REGISTRY_H
#define SHAPEREALITY_IMPORT_REGISTRY_H

#include <string>

namespace asset
{
    using ImportFunction = std::function<void(std::string const& sourceFilePath, std::string const& targetFilePath)>;

    // an ImportRegistry contains a mapping of asset types to file formats and their
    // corresponding import functions
    class ImportRegistry final // rename to AssetImporter
    {
    public:
        explicit ImportRegistry();
        ~ImportRegistry();

        void emplace(std::vector<std::string> const& fileExtensions, ImportFunction&& importFunction);

        // should automatically look for a meta file that accompanies the source file path
        void importAsset(std::string const& sourceFilePath, std::string const& targetFilePath);

    private:
        std::vector<ImportFunction> functions;

    };
}

#endif //SHAPEREALITY_IMPORT_REGISTRY_H
