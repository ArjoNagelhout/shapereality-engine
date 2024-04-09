//
// Created by Arjo Nagelhout on 09/04/2024.
//

#include "asset_info_registry.h"

#include <cassert>

namespace asset
{
    AssetInfoRegistry& AssetInfoRegistry::shared()
    {
        static AssetInfoRegistry instance;
        return instance;
    }

    void AssetInfoRegistry::emplace(std::unique_ptr<AssetInfo>&& info, reflection::TypeId typeId)
    {
        assert(!assetTypes.contains(typeId) && "can't register an asset type twice");
        assetTypes.emplace(typeId, std::move(info));
    }

    bool AssetInfoRegistry::contains(reflection::TypeId typeId) const
    {
        return assetTypes.contains(typeId);
    }

    AssetInfo* AssetInfoRegistry::get(reflection::TypeId typeId)
    {
        if (assetTypes.contains(typeId))
        {
            return assetTypes.at(typeId).get();
        }
        else
        {
            return nullptr;
        }
    }
}