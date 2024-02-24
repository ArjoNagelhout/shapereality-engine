//
// Created by Arjo Nagelhout on 31/01/2024.
//

#include <asset/asset_database.h>
#include <asset/import/gltf.h>
#include <reflection/serialize/json.h>

#include <iostream>

using namespace asset;

int main(int argc, char* argv[])
{
    fs::path inputDirectory(argv[1]);
    fs::path loadDirectory(argv[2]);

    reflection::TypeInfoRegistry r;
    reflection::JsonSerializer serializer(r);

    r.emplace<fs::path>({"fs::path"});
    serializer.emplace<fs::path>(
        [](nlohmann::json const& in, fs::path* out) { *out = in.get<std::string>(); },
        [](fs::path* in, nlohmann::json& out) { out = in->generic_string(); }
    );

    // we don't use std::format as clang does not implement it
    r.emplace<fs::file_time_type>({"file_time_type"});
    serializer.emplace<fs::file_time_type>(
        [](nlohmann::json const& in, fs::file_time_type* out) {
            unsigned int timeSinceEpoch = in.get<unsigned int>();
            *out = fs::file_time_type() + fs::file_time_type::duration(timeSinceEpoch);
        },
        [](fs::file_time_type* in, nlohmann::json& out) {
            out = static_cast<unsigned int>(in->time_since_epoch().count());
        }
    );

    reflection::TypeInfoBuilder<InputFile>("InputFile")
        .property<&InputFile::path>("path")
        .property<&InputFile::lastWriteTime>("lastWriteTime")
        .property<&InputFile::artifacts>("artifacts")
        .emplace(r);

    reflection::TypeInfoBuilder<AssetId>("AssetId")
        .property<&AssetId::inputFilePath>("inputFilePath")
        .property<&AssetId::artifactPath>("artifactPath")
        .emplace(r);

    ImportRegistry importers{};
    importers.emplace([](std::function<void()> const& onComplete) { std::cout << "texture import" << std::endl; },
                      {"jpg", "jpeg", "png"});
    importers.emplace([](std::function<void()> const& onComplete) { std::cout << "gltf" << std::endl; }, {"gltf"});
    AssetDatabase db(serializer, importers, inputDirectory, loadDirectory);

    db.importFile("models/sea_house/scene.gltf", [&](Result<InputFile*> const& result) {
        if (result.error())
        {
            std::cout << result.message() << std::endl;
            return;
        }
        InputFile& a = *result.get();
        std::cout << "imported " << a.path << ", lastWriteTime: "
                  << static_cast<int>(a.lastWriteTime.time_since_epoch().count()) << std::endl;
        std::string json = serializer.toJsonString(a, 2);
        std::cout << json << std::endl;
    });
    return 0;
}