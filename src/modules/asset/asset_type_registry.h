//
// Created by Arjo Nagelhout on 09/04/2024.
//

#ifndef SHAPEREALITY_ASSET_TYPE_REGISTRY_H
#define SHAPEREALITY_ASSET_TYPE_REGISTRY_H

#include "asset_type.h"

#include <reflection/type_id.h>

#include <unordered_map>


namespace asset
{
    /**
     *
     */
    class AssetTypeRegistry final
    {
    public:
        // shared instance
        [[nodiscard]] static AssetTypeRegistry& shared();

        void emplace(AssetType&& info, reflection::TypeId typeId);

        template<typename Type>
        void emplace(AssetType&& info)
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

        [[nodiscard]] AssetType* get(reflection::TypeId typeId);

        template<typename Type>
        [[nodiscard]] AssetType* get()
        {
            reflection::TypeId typeId = reflection::TypeIndex<Type>::value();
            return get(typeId);
        }

    private:
        std::unordered_map<reflection::TypeId, AssetType> assetTypes{};
    };
}

#define REGISTER_ASSET_TYPE void register_(asset::AssetTypeRegistry& assetTypes)

#endif //SHAPEREALITY_ASSET_TYPE_REGISTRY_H
