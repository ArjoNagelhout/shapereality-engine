//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_ASSET_HANDLE_H
#define SHAPEREALITY_ASSET_HANDLE_H

#include <string>

#include <common/result.h>

#include <renderer/mesh.h>
#include <graphics/texture.h>
#include <scene/scene.h>

#include "asset_id.h"

namespace asset
{
    /**
     * To support storing asset handles of different types
     * inside a container such as a std::vector, we create the
     * type AssetHandleBase
     */
    class AssetHandleBase
    {
    public:
        explicit AssetHandleBase(AssetId id);

        [[nodiscard]] AssetId const& id() const;

        // to support casting from the AssetHandleBase to a AssetHandle<Type>,
        // we store the typeId
        [[nodiscard]] reflection::TypeId typeId() const;

        template<typename Type>
        [[nodiscard]] bool isType() const
        {
            reflection::TypeId t = reflection::TypeIndex<Type>::value();
            return t == typeId_;
        }

        // if this AssetHandle's type is nullTypeId, it is considered empty
        // it does not contain a type (and thus can't be cast to a specific AssetHandle<Type>)
        [[nodiscard]] bool empty() const;

        [[nodiscard]] bool completed() const;

        [[nodiscard]] bool success() const;

        [[nodiscard]] bool error() const;

        [[nodiscard]] common::ResultCode code() const;

    protected:
        explicit AssetHandleBase(AssetId id, reflection::TypeId typeId);

    private:
        AssetId id_;
        reflection::TypeId typeId_; // used for checking type at runtime for casting
        bool completed_{};
        common::ResultCode code_{};
    };

    template<typename Type>
    class AssetHandle : public AssetHandleBase
    {
    public:
        template<typename... Args>
        explicit AssetHandle(AssetId id, Args&&... args)
            : AssetHandleBase(std::move(id), reflection::TypeIndex<Type>::value()), asset(std::forward<Args>(args)...)
        {

        }

        // delete copy constructor and assignment operator
        AssetHandle(AssetHandle const&) = delete;

        AssetHandle& operator=(AssetHandle const&) = delete;

        [[nodiscard]] Type& get()
        {
            return *asset.get();
        }

    private:
        Type asset;
    };

    // Asset is a shorthand for std::shared_ptr<AssetHandle>
    using AssetBase = std::shared_ptr<AssetHandleBase>;

    template<typename Type>
    using Asset = std::shared_ptr<AssetHandle<Type>>;

    // the returned typed asset shares ownership with the provided untyped asset
    template<typename Type>
    [[nodiscard]] Asset<Type> cast(AssetBase const& asset)
    {
        assert(asset->isType<Type>() && "asset is not of type, make sure you check the type before calling this function");
        return std::static_pointer_cast<Type>(asset);
    }

    template<typename Type>
    [[nodiscard]] AssetBase castToUntyped(Asset<Type> const& asset)
    {
        return std::static_pointer_cast<AssetBase>(asset);
    }

    /**
         * @tparam Type the asset type to use
         * @param args arguments for constructing the AssetHandle, this is the AssetId and
         * arguments for the constructor of the Type
         * @return a shared pointer to the constructed asset
         */
    template<typename Type, typename... Args>
    std::shared_ptr<AssetHandle<Type>> makeAsset(Args&&... args)
    {
        return std::make_shared<AssetHandle<Type>>(std::forward<Args>(args)...);
    }
}

#endif //SHAPEREALITY_ASSET_HANDLE_H
