//
// Created by Arjo Nagelhout on 16/03/2024.
//

#include <gtest/gtest.h>

#include <asset/asset_database.h>
#include <asset/import/gltf.h>
#include <asset/reflection.h>
#include <common/thread_pool.h>

#include <common/application_info.h>
#include <common/logger.h>

#include <cstdlib>

using namespace asset;

namespace asset_database_test
{
    class AssetDatabaseObserver : public IAssetDatabaseObserver
    {
    public:
        explicit AssetDatabaseObserver(int i_) : i(i_) {}

        std::string prefix() const
        {
            return std::string("observer ") + std::to_string(i) + ": ";
        }

        void onImportComplete(std::filesystem::path const& inputFile, ImportResult result) override
        {
            std::cout << prefix() << "import complete" << std::endl;
        }

        void onImportStarted(std::filesystem::path const& inputFile) override
        {
            std::cout << prefix() << "import started for " << inputFile << std::endl;
        }

    private:
        int i;
    };

    TEST(AssetDatabase, Main)
    {
        // program arguments
        std::filesystem::path inputDirectory("/Users/arjonagelhout/Documents/ShapeReality/project/input_directory");
        std::filesystem::path loadDirectory("/Users/arjonagelhout/Documents/ShapeReality/project/load_directory");

        asset::ImportRegistry importers;
        importers.emplace(asset::importGltfNew, {"gltf"});

        // reflection
        asset::registerReflection();

        AssetDatabase assets{
            common::ThreadPool::shared(),
            reflection::JsonSerializer::shared(),
            importers,
            inputDirectory,
            loadDirectory,
            false};

        AssetDatabaseObserver observer{0};
        assets.observers.add(&observer);

        assets.importFile("models/sea_house/scene.gltf");
        assets.importFile("scene_invalid.gltf");
        assets.importFile("scene.gltf");
    }
}