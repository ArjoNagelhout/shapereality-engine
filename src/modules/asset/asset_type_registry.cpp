//
// Created by Arjo Nagelhout on 09/04/2024.
//

#include "asset_type_registry.h"

#include <cassert>

namespace asset
{
    AssetTypeRegistry& AssetTypeRegistry::shared()
    {
        static AssetTypeRegistry instance;
        return instance;
    }

    void AssetTypeRegistry::emplace(AssetType&& info, reflection::TypeId typeId)
    {
        assert(!assetTypes.contains(typeId) && "can't register an asset type twice");
        assetTypes.emplace(typeId, std::move(info));
    }

    bool AssetTypeRegistry::contains(reflection::TypeId typeId) const
    {
        return assetTypes.contains(typeId);
    }

    AssetType const& AssetTypeRegistry::get(reflection::TypeId typeId) const
    {
        assert(assetTypes.contains(typeId) && "AssetType should be registered");
        return assetTypes.at(typeId);
    }
}