//
// Created by Arjo Nagelhout on 08/04/2024.
//


#include <gtest/gtest.h>

#include <asset/asset_handle.h>

namespace handles_test
{
    struct AssetType1
    {

    };

    struct AssetType2
    {

    };

    struct AssetType3
    {

    };

    TEST(Asset, Handles)
    {
        asset::Asset a = asset::makeAsset<AssetType1>(asset::AssetId{});
        asset::Asset b = asset::makeAsset<AssetType2>(asset::AssetId{});
        asset::Asset c = asset::makeAsset<AssetType3>(asset::AssetId{});

        ASSERT_TRUE(a->isType<AssetType1>());
        ASSERT_TRUE(b->isType<AssetType2>());
        ASSERT_FALSE(c->isType<AssetType2>());
    }
}