//
// Created by Arjo Nagelhout on 09/04/2024.
//

#ifndef SHAPEREALITY_ASSET_TYPE_REGISTRY_H
#define SHAPEREALITY_ASSET_TYPE_REGISTRY_H

#include "asset_type.h"
#include "asset_id.h"

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

        [[nodiscard]] AssetType const& get(reflection::TypeId typeId) const;

        template<typename Type>
        [[nodiscard]] AssetType const& get() const
        {
            reflection::TypeId typeId = reflection::TypeIndex<Type>::value();
            return get(typeId);
        }

        // convenience function for making an asset id for a given artifact name with a specific type
        // automatically adds the file extension
        template<typename Type, typename... Args>
        [[nodiscard]] AssetId makeAssetId(std::filesystem::path const& inputFile, fmt::format_string<Args...> fmt, Args&&... args) const
        {
            return AssetId{
                .inputFilePath = inputFile,
                .artifactPath = fmt::format("{}.{}", fmt::format(fmt, std::forward<Args>(args)...), get<Type>().fileExtension)
            };
        }

    private:
        std::unordered_map<reflection::TypeId, AssetType> assetTypes{};
    };
}

#endif //SHAPEREALITY_ASSET_TYPE_REGISTRY_H
