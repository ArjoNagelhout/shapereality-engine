#include <graphics/application.h>
#include <graphics/graphics.h>
#include <graphics/device.h>
#include <graphics/window.h>

#include <asset/asset_database.h>

#include <asset/reflection.h>
#include <scene/reflection.h>
#include <entity/reflection.h>

#include "editor.h"

int main(int argc, char* argv[])
{
    // program arguments
    fs::path inputDirectory(argv[1]);
    fs::path loadDirectory(argv[2]);

    // reflection
    asset::registerReflection();
    scene::registerReflection();
    entity::registerReflection();

    // application
    graphics::Application application{};
    editor::Editor editor(inputDirectory, loadDirectory);
    application.setDelegate(&editor);

    // graphics backend
    graphics::GraphicsBackend backend = graphics::GraphicsBackend::Metal;
    std::unique_ptr<graphics::IDevice> device = graphics::createDevice(backend);

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
