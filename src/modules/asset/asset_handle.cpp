//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "asset_handle.h"

#include <utility>

namespace asset
{
    AssetHandle::AssetHandle(AssetId id) : id_(std::move(id)) {}

    AssetHandle::~AssetHandle() = default;

    AssetId const& AssetHandle::id() const
    {
        return id_;
    }

    bool AssetHandle::completed() const
    {
        return completed_;
    }

    bool AssetHandle::success() const
    {
        return code_ == common::ResultCode::Success;
    }

    bool AssetHandle::error() const
    {
        return code_ != common::ResultCode::Success;
    }

    common::ResultCode AssetHandle::code() const
    {
        return code_;
    }
}