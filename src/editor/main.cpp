#include <graphics/application.h>
#include <graphics/types.h>
#include <graphics/device.h>
#include <graphics/window.h>

#include <asset/asset_database.h>

#include <reflection/type_info_registry.h>
#include <reflection/serialize/json.h>
#include <import/gltf/register.h>
#include <asset/register.h>
#include <scene/register.h>
#include <entity/register.h>

#include "editor.h"

int main(int argc, char* argv[])
{
    // program arguments
    std::filesystem::path inputDirectory(argv[1]);
    std::filesystem::path loadDirectory(argv[2]);

    // reflection
    reflection::TypeInfoRegistry types;
    reflection::JsonSerializer json{types};
    asset::register_(types, json);
    scene::register_(types, json);
    entity::register_(types, json);
    import_::gltf::register_(types, json);

    // application
    graphics::Application application{};
    editor::Editor editor(inputDirectory, loadDirectory);
    application.setDelegate(&editor);

    // graphics device
    std::unique_ptr<graphics::IDevice> device = graphics::createDevice();

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
