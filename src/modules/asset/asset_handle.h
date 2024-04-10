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
    struct AssetHandleDataBase
    {
    };

    template<typename Type>
    struct AssetHandleData;

    /**
     * Contains a std::unique_ptr<Type> to an AssetHandleData
     * we could also store this as std::any
     */
    class AssetHandle final
    {
    public:
        enum class State
        {
            Uninitialized = 0,
            Loading,
            Done
        };

        // construct empty untyped AssetHandle
        explicit AssetHandle(AssetId id);

        // construct empty typed AssetHandle
        template<typename Type>
        explicit AssetHandle(AssetId id) : AssetHandle(std::move(id))
        {
            typeId_ = reflection::TypeIndex<Type>::value();
        }

        // construct typed AssetHandle with data
        template<typename Type, typename... Args>
        explicit AssetHandle(AssetId id, Args&& ... args) : AssetHandle(std::move(id))
        {
            set<Type>(std::forward<Args>(args)...);
        }

        // delete copy constructor and assignment operator
        AssetHandle(AssetHandle const&) = delete;

        AssetHandle& operator=(AssetHandle const&) = delete;

        //
        [[nodiscard]] AssetId const& id() const;

        // to support casting from the AssetHandleBase to a AssetHandle<Type>, we store the typeId
        [[nodiscard]] reflection::TypeId typeId() const;

        // whether the asset handle is loading or completed
        [[nodiscard]] State state() const;

        //
        [[nodiscard]] common::ResultCode code() const;

        //
        [[nodiscard]] bool success() const;

        //
        [[nodiscard]] bool error() const;

        // return whether the provided template argument's type is the same type as what is stored in this AssetHandle
        // if the AssetHandle is empty, but typed (i.e. constructed with the templated constructor, this returns true)
        template<typename Type>
        [[nodiscard]] bool isType() const
        {
            reflection::TypeId t = reflection::TypeIndex<Type>::value();
            return t == typeId_;
        }

        // returns whether this AssetHandle contains valid data for the provided `Type` template argument
        template<typename Type>
        [[nodiscard]] bool valid() const
        {
            if (state_ != State::Done || code_ != common::ResultCode::Success)
            {
                return false;
            }

            if (!isType<Type>())
            {
                return false;
            }

            if (!data)
            {
                return false;
            }

            return true;
        }

        // only use if you have checked whether the AssetHandle is valid using valid()
        template<typename Type>
        [[nodiscard]] Type& get() const
        {
            assert(valid<Type>() && "AssetHandle should be valid when calling get()");
            return *std::static_pointer_cast<AssetHandleData<Type>>().get();
        }

        // takes ownership of a provided unique_ptr
        template<typename Type>
        void set(std::unique_ptr<AssetHandleData<Type>>&& data_)
        {
            assert(data_ && "data should not be nullptr");
            data = std::move(data_);
            onSet(reflection::TypeIndex<Type>::value());
        }

        template<typename Type, typename... Args>
        void set(Args&& ... args)
        {
            data = std::make_unique<AssetHandleData<Type>>(std::forward<Args>(args)...);
            onSet(reflection::TypeIndex<Type>::value());
        }

        // set the asset handle into an error state, indicating that importing the asset with this AssetId was unsuccessful
        void setError(common::ResultCode code);

    private:
        AssetId id_;
        reflection::TypeId typeId_; // used for checking type at runtime for casting
        State state_ = State::Uninitialized;
        common::ResultCode code_ = common::ResultCode::Unknown;

        std::unique_ptr<AssetHandleDataBase> data;

        void onSet(reflection::TypeId typeId);
    };

    template<typename Type>
    class AssetHandleData : public AssetHandleDataBase
    {
    public:
        Type data;

    private:
        // construct AssetHandleData with data
        template<typename... Args>
        explicit AssetHandleData(Args&& ... args) : data(std::forward<Args>(args)...)
        {

        }

        friend class AssetHandle;
    };

    // Asset is a shorthand for std::shared_ptr<AssetHandle>
    using Asset = std::shared_ptr<AssetHandle>;

//    template<typename Type, typename... Args>
//    std::shared_ptr<AssetHandle> makeAsset(AssetId id, Args&& ... args)
//    {
//        return std::make_shared<AssetHandle>(std::forward<Args>(args)...);
//    }
}

#endif //SHAPEREALITY_ASSET_HANDLE_H
