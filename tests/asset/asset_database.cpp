//
// Created by Arjo Nagelhout on 16/03/2024.
//

#include <gtest/gtest.h>

#include <asset/asset_database.h>
#include <asset/reflection.h>
#include <common/thread_pool.h>

using namespace asset;

namespace asset_database_test
{
    class AssetDatabaseObserver : public IAssetDatabaseObserver
    {
        void onImportComplete() override
        {
            std::cout << "observer: import complete" << std::endl;
        }

        void onImportStarted() override
        {
            std::cout << "observer: import started" << std::endl;
        }
    };

    TEST(AssetDatabase, Main)
    {
        // program arguments
        fs::path inputDirectory("/Users/arjonagelhout/Documents/ShapeReality/project/input_directory");
        fs::path loadDirectory("/Users/arjonagelhout/Documents/ShapeReality/project/load_directory");

        asset::ImportRegistry importers;

        // reflection
        asset::registerReflection();

        AssetDatabase assets{
            common::ThreadPool::shared(),
            reflection::JsonSerializer::shared(),
            importers,
            inputDirectory,
            loadDirectory,
            false};

        AssetDatabaseObserver observer;
        assets.observers.add(&observer);

        assets.importFile("models/sea_house/scene.glt");
        assets.importFile("models/sea_house/scene.gltf");
    }
}