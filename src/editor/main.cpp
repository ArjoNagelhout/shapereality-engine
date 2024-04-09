#include <graphics/application.h>
#include <graphics/types.h>
#include <graphics/device.h>
#include <graphics/window.h>

#include <asset/asset_database.h>

#include <reflection/type_registry.h>
#include <reflection/serialize/json.h>
#include <reflection/serialize/binary.h>
#include <import/gltf/register.h>
#include <asset/register.h>
#include <scene/register.h>
#include <entity/register.h>
#include <renderer/register.h>

#include "editor.h"

int main(int argc, char* argv[])
{
    // program arguments
    std::filesystem::path inputDirectory(argv[1]);
    std::filesystem::path loadDirectory(argv[2]);

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

    // application
    graphics::Application application{};

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

    // entry point for our Editor app
    editor::Editor editor{assets};
    application.setDelegate(&editor);

    // create window
    graphics::WindowDescriptor descriptor{
        .x = 600,
        .y = 500,
        .width = 1300,
        .height = 700,
        .flags = graphics::WindowFlags_Default,
        .clearColor = math::Vector4{{0.5f, 1.f, 1.f, 1.f}}
    };
    std::unique_ptr<graphics::Window> window = device->createWindow(descriptor);
    window->setTitle("Engine somewhat from scratch");
    window->setMinSize({300, 100});
    window->setRenderDelegate(&editor);
    window->setInputDelegate(&editor);

    editor.device = device.get();
    editor.window = window.get();

    application.run();

    return 0;
}
