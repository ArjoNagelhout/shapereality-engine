//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "asset_handle.h"

#include <utility>

namespace asset
{
    AssetHandle::AssetHandle(AssetId assetId_) : assetId(std::move(assetId_))
    {

    }

    AssetHandle::~AssetHandle() = default;
}