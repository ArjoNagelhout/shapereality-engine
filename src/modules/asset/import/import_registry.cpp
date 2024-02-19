//
// Created by Arjo Nagelhout on 18/02/2024.
//

#include "import_registry.h"

namespace asset
{
    ImportRegistry::ImportRegistry() = default;

    ImportRegistry::~ImportRegistry() = default;

    void ImportRegistry::emplace(std::vector<std::string> const& fileExtensions, ImportFunction&& importFunction)
    {

    }
}