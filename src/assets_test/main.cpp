//
// Created by Arjo Nagelhout on 31/01/2024.
//

#include <asset/asset_database.h>
#include <asset/import/gltf.h>
#include <iostream>

using namespace asset;

int main(int argc, char* argv[])
{
    fs::path inputDirectory(argv[1]);
    fs::path loadDirectory(argv[2]);

    ImportRegistry importers{};
    importers.emplace([](std::function<void()> const& onComplete) { std::cout << "texture import" << std::endl; }, {"jpg", "jpeg", "png"});
    importers.emplace([](std::function<void()> const& onComplete) { std::cout << "gltf" << std::endl; }, {"gltf"});
    AssetDatabase db(importers, inputDirectory, loadDirectory);

    db.importFile("models/sea_house/scene.gltf", [](Result<InputFile*> const& result) {
        if (result.error())
        {
            std::cout << result.message() << std::endl;
            return;
        }
        InputFile& a = *result.get();
        std::cout << "imported " << a.path << std::endl;
    });
    return 0;
}