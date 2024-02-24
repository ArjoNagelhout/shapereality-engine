//
// Created by Arjo Nagelhout on 31/01/2024.
//

#include <asset/asset_database.h>
#include <asset/import/gltf.h>
#include <iostream>

#include <reflection/serialize/json.h>

using namespace asset;

int main(int argc, char* argv[])
{
    fs::path inputDirectory(argv[1]);
    fs::path loadDirectory(argv[2]);

    reflection::TypeInfoRegistry r;
    reflection::JsonSerializer serializer(r);

    //r.emplace<fs::path>({"fs::path"});
    serializer.emplace<fs::path>(
        [](nlohmann::json const& in, fs::path* out) { *out = in.get<std::string>(); },
        [](fs::path* in, nlohmann::json& out) { out = in->generic_string(); }
    );

    reflection::TypeInfoBuilder<InputFile>("InputFile")
        .property<&InputFile::path>("path")
//        .property<&InputFile::lastWriteTime>("lastWriteTime")
        .property<&InputFile::artifacts>("artifacts")
        .emplace(r);

    InputFile inputFile{
        .path = "something/test.ha",
        .artifacts{},
        .lastWriteTime{},
    };

    std::cout << serializer.toJsonString(inputFile) << std::endl;

    ImportRegistry importers{};
    importers.emplace([](std::function<void()> const& onComplete) { std::cout << "texture import" << std::endl; },
                      {"jpg", "jpeg", "png"});
    importers.emplace([](std::function<void()> const& onComplete) { std::cout << "gltf" << std::endl; }, {"gltf"});
    AssetDatabase db(serializer, importers, inputDirectory, loadDirectory);

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