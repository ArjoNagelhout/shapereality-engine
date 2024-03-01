#include <graphics/application.h>
#include <graphics/graphics.h>
#include <graphics/device.h>
#include <graphics/window.h>

#include <asset/asset_database.h>

#include <BS_thread_pool.hpp>
#include <iostream>

#include <asset/reflection.h>

#include "editor.h"

int main(int argc, char* argv[])
{
    fs::path inputDirectory(argv[1]);
    fs::path loadDirectory(argv[2]);

    // application
    graphics::Application application{};

    // thread pool
    BS::thread_pool threadPool;
    std::cout << "created thread pool with size " << threadPool.get_thread_count() << std::endl;

    // reflection
    TypeInfoRegistry types;
    EnumSerializer enums;
    JsonSerializer jsonSerializer(types, enums);

    // register types for reflection
    asset::registerTypes(types, jsonSerializer);

    editor::Editor editor(threadPool, jsonSerializer, inputDirectory, loadDirectory);
    application.setDelegate(&editor);

    graphics::GraphicsBackend backend = graphics::GraphicsBackend::Metal;
    std::unique_ptr<graphics::IDevice> device = graphics::createDevice(backend);

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
