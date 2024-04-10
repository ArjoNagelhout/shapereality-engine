//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "asset_handle.h"

#include <utility>

namespace asset
{
    AssetHandle::AssetHandle(asset::AssetId id)
        : id_(std::move(id)), typeId_(reflection::nullTypeId)
    {

    }

    AssetId const& AssetHandle::id() const
    {
        return id_;
    }

    reflection::TypeId AssetHandle::typeId() const
    {
        return typeId_;
    }

    AssetHandle::State AssetHandle::state() const
    {
        return state_;
    }

    common::ResultCode AssetHandle::code() const
    {
        return code_;
    }

    bool AssetHandle::success() const
    {
        assert(state_ == State::Done && "state should be Done when checking for success()");
        return code_ == common::ResultCode::Success;
    }

    bool AssetHandle::error() const
    {
        assert(state_ == State::Done && "state should be Done when checking for error()");
        return code_ != common::ResultCode::Success;
    }

    void AssetHandle::onSet(reflection::TypeId typeId)
    {
        typeId_ = typeId;
        state_ = State::Done;
        code_ = common::ResultCode::Success;
    }

    void AssetHandle::setError(common::ResultCode code)
    {
        typeId_ = reflection::nullTypeId;
        state_ = State::Done;
        code_ = code;
        data.reset();
    }
}