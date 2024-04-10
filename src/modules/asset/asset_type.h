//
// Created by Arjo Nagelhout on 08/04/2024.
//

#ifndef SHAPEREALITY_ASSET_TYPE_H
#define SHAPEREALITY_ASSET_TYPE_H

#include <string>
#include <fmt/format.h>

namespace asset
{
    /*
     * data for deserializing and serializing a specific asset
     * type. gets registered in a `AssetInfoRegistry`
     */
    struct AssetType
    {
        std::string const fileExtension;

        template<typename... Args>
        [[nodiscard]] std::string getFileName(fmt::format_string<Args...> fmt, Args&&... args) const
        {
            return getFileNameImplementation(fmt::format(fmt, std::forward<Args>(args)...));
        }

    private:
        // returns file name with extension
        [[nodiscard]] std::string getFileNameImplementation(std::string const& name) const;
    };
}

#endif //SHAPEREALITY_ASSET_TYPE_H
