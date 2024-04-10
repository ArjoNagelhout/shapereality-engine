//
// Created by Arjo Nagelhout on 10/04/2024.
//

#include "asset_type.h"

namespace asset
{
    [[nodiscard]] std::string AssetType::getFileNameImplementation(std::string const& name) const
    {
        return fmt::format("{}.{}", name, fileExtension);
    }
}