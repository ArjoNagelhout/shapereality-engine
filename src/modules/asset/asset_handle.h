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
#include <memory>

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
        enum class State
        {
            Uninitialized,
            Loading, // gets set by
            Completed
        };

        // constructs empty untyped AssetHandle
        explicit AssetHandleBase(AssetId id);

        [[nodiscard]] AssetId const& id() const;

        // to support casting from the AssetHandleBase to a AssetHandle<Type>, we store the typeId
        [[nodiscard]] reflection::TypeId typeId() const;

        template<typename Type>
        [[nodiscard]] bool isType() const
        {
            reflection::TypeId t = reflection::TypeIndex<Type>::value();
            return t == typeId_;
        }

        [[nodiscard]] State state() const;

        [[nodiscard]] bool success() const;

        [[nodiscard]] bool error() const;

        [[nodiscard]] common::ResultCode code() const;

    protected:
        explicit AssetHandleBase(AssetId id, reflection::TypeId typeId);

    private:
        AssetId id_;
        reflection::TypeId typeId_; // used for checking type at runtime for casting
        State state_ = State::Uninitialized;
        common::ResultCode code_ = common::ResultCode::Unknown;
    };

    template<typename Type>
    class AssetHandle : public AssetHandleBase
    {
    public:
        // constructs empty AssetHandle, to be filled later
        explicit AssetHandle(AssetId id) : AssetHandleBase(std::move(id), reflection::TypeIndex<Type>::value())
        {

        }

        template<typename... Args>
        explicit AssetHandle(AssetId id, Args&&... args)
            : AssetHandleBase(std::move(id), reflection::TypeIndex<Type>::value()), asset(std::make_unique<Type>(std::forward<Args>(args)...))
        {

        }

        // delete copy constructor and assignment operator
        AssetHandle(AssetHandle const&) = delete;

        AssetHandle& operator=(AssetHandle const&) = delete;

        [[nodiscard]] Type& get()
        {
            assert(success() && "get() should only be called on an AssetHandle that has successfully loaded, call success() to test");
            assert(asset && "if success() is true, asset should be set");
            return *asset.get();
        }

    private:
        // we store the asset in a unique_ptr to support returning a typed asset handle that is not initialized yet, and can later
        // be populated by the import function if it exists in "assets" in the AssetDatabase
        std::unique_ptr<Type> asset;
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
        return std::static_pointer_cast<AssetHandle<Type>>(asset);
    }

    template<typename Type>
    [[nodiscard]] AssetBase castToUntyped(Asset<Type> const& asset)
    {
        return std::static_pointer_cast<AssetHandleBase>(asset);
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
