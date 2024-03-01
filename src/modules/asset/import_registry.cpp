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

    ImportRegistry::~ImportRegistry() = default;

    void ImportRegistry::emplace(ImportFunction&& function, std::vector<std::string> const& extensions_)
    {
        for (auto& extension: extensions_)
        {
            std::string const e = removeLeadingDot(extension);
            assert(!extensions.contains(e));

            // we take the size before it was added, so we don't have to subtract one
            extensions.emplace(e, functions.size());
        }
        functions.emplace_back(function);
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