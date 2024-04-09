//
// Created by Arjo Nagelhout on 09/04/2024.
//

#ifndef SHAPEREALITY_ASSET_INFO_REGISTRY_H
#define SHAPEREALITY_ASSET_INFO_REGISTRY_H

#include "asset_info.h"

#include <reflection/type_id.h>

#include <unordered_map>


namespace asset
{
    /**
     *
     */
    class AssetTypeInfoRegistry final
    {
    public:
        // shared instance
        [[nodiscard]] static AssetTypeInfoRegistry& shared();

        void emplace(std::unique_ptr<AssetInfo>&& info, reflection::TypeId typeId);

        template<typename Type>
        void emplace(std::unique_ptr<AssetInfo>&& info)
        {
            reflection::TypeId typeId = reflection::TypeIndex<Type>::value();
            emplace(std::move(info), typeId);
        }

        [[nodiscard]] bool contains(reflection::TypeId typeId) const;

        template<typename Type>
        [[nodiscard]] bool contains() const
        {
            reflection::TypeId typeId = reflection::TypeIndex<Type>::value();
            return contains(typeId);
        }

        [[nodiscard]] AssetInfo* get(reflection::TypeId typeId);

        template<typename Type>
        [[nodiscard]] AssetInfo* get()
        {
            reflection::TypeId typeId = reflection::TypeIndex<Type>::value();
            return get(typeId);
        }

    private:
        std::unordered_map<reflection::TypeId, std::unique_ptr<AssetInfo>> assetTypes{};
    };
}

#define REGISTER_ASSET_TYPE void register_(asset::AssetTypeInfoRegistry& assetTypes)

#endif //SHAPEREALITY_ASSET_INFO_REGISTRY_H
