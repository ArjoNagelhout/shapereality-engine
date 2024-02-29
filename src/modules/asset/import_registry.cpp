//
// Created by Arjo Nagelhout on 26/02/2024.
//

#include "import_registry.h"

#include <cassert>
#include <iostream>

namespace asset
{
    [[nodiscard]] std::string removeLeadingDot(std::string string)
    {
        if (string.starts_with('.'))
        {
            string = string.substr(1);
        }
        return string;
    }

    void ImportRegistry::emplace(ImportFunction&& function, std::vector<std::string> const& _extensions)
    {
        ImportFunction& f = functions.emplace_back(function);
        for (auto& extension: _extensions)
        {
            extensions.emplace(removeLeadingDot(extension), f);
        }
    }

    bool ImportRegistry::contains(std::string const& extension)
    {
        return extensions.contains(removeLeadingDot(extension));
    }

    void ImportRegistry::importFile(fs::path const& absolutePath)
    {
        std::cout << absolutePath << std::endl;
        assert(absolutePath.has_extension());
        std::string extension = removeLeadingDot(absolutePath.extension());
        assert(contains(extension));
        extensions.at(extension)(absolutePath);
    }
}