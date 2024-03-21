//
// Created by Arjo Nagelhout on 16/03/2024.
//

#include <gtest/gtest.h>

#include <asset/asset_database.h>
#include <asset/reflection.h>
#include <common/thread_pool.h>

#include <common/application_info.h>

#include <cstdlib>

using namespace asset;

namespace asset_database_test
{
    ImportResult importGltfDummy(AssetDatabase& assets, std::filesystem::path const& inputFile)
    {
        return ImportResult::makeError(common::ResultCode::Unimplemented, "Import gltf not implemented");
    }

    class AssetDatabaseObserver : public IAssetDatabaseObserver
    {
    public:
        explicit AssetDatabaseObserver(int i_) : i(i_) {}

        std::string prefix()
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

        AssetDatabaseObserver observer{0};
        AssetDatabaseObserver observer1{1};
        AssetDatabaseObserver observer2{2};
        assets.observers.add(&observer);
        assets.observers.add(&observer1);
        assets.observers.add(&observer2);

        assets.observers.remove(&observer);

        importers.emplace(importGltfDummy, {"gltf"});

        assets.importFile("models/sea_house/scene.glt");
        assets.importFile("models/sea_house/scene.gltf");

        std::cout << "user home directory: " << common::ApplicationInfo::userHomeDirectory() << std::endl;
        std::cout << "logging directory: " << common::ApplicationInfo::loggingDirectory() << std::endl;
        std::cout << "persistent directory: " << common::ApplicationInfo::persistentDataDirectory() << std::endl;

        std::cout << "identifier: " << common::ApplicationInfo::bundleIdentifier() << std::endl;
    }
}