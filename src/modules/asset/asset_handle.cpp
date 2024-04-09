//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "asset_handle.h"

#include <utility>

namespace asset
{
    AssetHandleBase::AssetHandleBase(asset::AssetId id)
        : AssetHandleBase(std::move(id), reflection::nullTypeId)
    {

    }

    AssetHandleBase::AssetHandleBase(AssetId id, reflection::TypeId typeId)
        : id_(std::move(id)), typeId_(typeId)
    {

    }

    AssetId const& AssetHandleBase::id() const
    {
        return id_;
    }

    bool AssetHandleBase::empty() const
    {
        return typeId_ == reflection::nullTypeId;
    }

    bool AssetHandleBase::completed() const
    {
        return completed_;
    }

    bool AssetHandleBase::success() const
    {
        return code_ == common::ResultCode::Success;
    }

    bool AssetHandleBase::error() const
    {
        return code_ != common::ResultCode::Success;
    }

    common::ResultCode AssetHandleBase::code() const
    {
        return code_;
    }
}