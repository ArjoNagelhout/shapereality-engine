//
// Created by Arjo Nagelhout on 16/03/2024.
//

#include <gtest/gtest.h>

#include <asset/asset_database.h>
#include <import/gltf/gltf.h>
#include <asset/register.h>
#include <renderer/register.h>
#include <import/gltf/register.h>
#include <scene/register.h>
#include <entity/register.h>

#include <common/application_info.h>

using namespace asset;

namespace asset_database_test
{
    class AssetDatabaseObserver : public IAssetDatabaseObserver
    {
    public:
        explicit AssetDatabaseObserver(int i_) : i(i_) {}

        [[nodiscard]] std::string prefix() const
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

        // reflection
        reflection::Reflection& reflection = reflection::Reflection::shared();
        asset::register_(reflection);
        scene::register_(reflection);
        entity::register_(reflection);
        renderer::register_(reflection);
        import_::gltf::register_(reflection);

        // import
        asset::ImportRegistry importers{};
        import_::gltf::register_(importers);

        // asset types
        asset::AssetTypeRegistry assetTypes{};
        renderer::register_(assetTypes);

        // graphics device
        std::unique_ptr<graphics::IDevice> device = graphics::createDevice();

        // asset database
        asset::AssetDatabaseParameters parameters{
            .inputDirectory = inputDirectory,
            .loadDirectory = loadDirectory,
            .useCache = false,
        };
        asset::AssetDatabaseContext context{
            .importers = importers,
            .assetTypes = assetTypes,
            .device = device.get()
        };
        asset::AssetDatabase assets{parameters, context};

        AssetDatabaseObserver observer{0};
        assets.observers.add(&observer);

        assets.importFile("models/sea_house/scene.gltf");

        // when calling .get<renderer::Mesh_>(), but we don't have a mesh yet, how do we handle this?
        // do we nest a unique_ptr inside the shared_ptr? That would allow us to have an uninitialized, typed
        // asset handle. 

        //auto a = assets.get<renderer::Mesh_>(AssetId{"models/sea_house/scene.gltf", ""});
        //assets.importFile("scene_invalid.gltf");
//        assets.importFile("scene.gltf");
    }
}