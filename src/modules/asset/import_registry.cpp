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

    ImportRegistry::~ImportRegistry()
    {
        std::cout << "whoops, already destroyed" << std::endl;
    }

    void ImportRegistry::emplace(ImportFunction&& function, std::vector<std::string> const& _extensions)
    {
        functions.emplace_back(function);
        for (auto& extension: _extensions)
        {
            extensions.emplace(removeLeadingDot(extension), functions.size()-1);
        }
    }

    bool ImportRegistry::contains(std::string const& extension)
    {
        return extensions.contains(removeLeadingDot(extension));
    }

    std::vector<AssetId> ImportRegistry::importFile(fs::path const& absolutePath)
    {
        assert(absolutePath.has_extension());
        std::string extension = removeLeadingDot(absolutePath.extension());
        if (!extensions.contains(extension))
        {
            // error: forgot to check if file extension is supported
            return {};
        }
        auto& f = functions.at(extensions.at(extension));
        return f(absolutePath);
    }
}