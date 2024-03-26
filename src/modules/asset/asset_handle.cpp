//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "asset_handle.h"

#include <utility>

namespace asset
{
    AssetHandleBase::AssetHandleBase(AssetId id) : id_(std::move(id)) {}

    AssetHandleBase::~AssetHandleBase() = default;

    AssetId const& AssetHandleBase::id() const
    {
        return id_;
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