//
// Created by Arjo Nagelhout on 26/02/2024.
//

#include "asset_id.h"

namespace asset
{
    std::string AssetId::string() const
    {
        return "{ inputFilePath: " + inputFilePath.string() + ", artifactPath: " + artifactPath.string() + "}";
    }

    bool operator==(AssetId const& lhs, AssetId const& rhs)
    {
        return lhs.artifactPath == rhs.artifactPath &&
               lhs.inputFilePath == rhs.inputFilePath;
    }
}