//
// Created by Arjo Nagelhout on 27/03/2024.
//

#include <gtest/gtest.h>

#include <asset/import_registry.h>
#include <asset/asset_database.h>

namespace move_import_result_data
{
    asset::ImportResultData getData()
    {
        return asset::ImportResultData{};
    }

    struct A
    {
        explicit A(asset::ImportResultData&& data_)
            : data(std::move(data_))
        {

        }

        asset::ImportResultData data;
    };

    A getA()
    {
        asset::ImportResultData data = getData();
//        data.artifacts.emplace_back(asset::makeAsset<bool>(asset::AssetId{"a.gltf", "mesh_001.mesh"}));
//        data.artifacts.emplace_back(asset::makeAsset<bool>(asset::AssetId{"a.gltf", "mesh_002.mesh"}));
//        data.artifacts.emplace_back(asset::makeAsset<bool>(asset::AssetId{"a.gltf", "mesh_003.mesh"}));
//        data.artifacts.emplace_back(asset::makeAsset<bool>(asset::AssetId{"a.gltf", "mesh_004.mesh"}));
        return A{std::move(data)};
    }

    TEST(AssetDatabase, MoveImportResultData)
    {
        // test why it produces a SIGABRT
        A a = getA();
        std::cout << a.data.artifacts.size() << std::endl;
    }
}