//
// Created by Arjo Nagelhout on 26/02/2024.
//

#include "import_registry.h"

namespace asset
{
    void ImportRegistry::emplace(ImportFunction&& function, std::vector<std::string> const& _extensions)
    {
        ImportFunction& f = functions.emplace_back(function);
        for (auto& extension: _extensions)
        {
            extensions.emplace(extension, f);
        }
    }

    bool ImportRegistry::contains(std::string const& extension)
    {
        std::string e = extension;
        if (extension.starts_with('.'))
        {
            e = e.substr(1);
        }
        return extensions.contains(e);
    }
}