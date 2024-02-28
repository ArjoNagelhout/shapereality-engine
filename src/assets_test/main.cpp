//
// Created by Arjo Nagelhout on 31/01/2024.
//

#include <asset/asset_database.h>
#include <asset/import/gltf.h>
#include <reflection/serialize/json.h>

#include <iostream>
#include <thread>
#include <BS_thread_pool.hpp>

using namespace asset;

void pathFromJson(nlohmann::json const& in, fs::path* out)
{
    *out = in.get<std::string>();
}

void pathToJson(fs::path* in, nlohmann::json& out)
{
    out = in->generic_string();
}

void fileTimeFromJson(nlohmann::json const& in, fs::file_time_type* out)
{
    unsigned int timeSinceEpoch = in.get<unsigned int>();
    *out = fs::file_time_type() + fs::file_time_type::duration(timeSinceEpoch);
}

void fileTimeToJson(fs::file_time_type* in, nlohmann::json& out)
{
    out = static_cast<unsigned int>(in->time_since_epoch().count());
}

int main(int argc, char* argv[])
{
    fs::path inputDirectory(argv[1]);
    fs::path loadDirectory(argv[2]);

    BS::thread_pool threadPool = BS::thread_pool();
    std::cout << "created thread pool with " << threadPool.get_thread_count() << " threads" << std::endl;

    reflection::TypeInfoRegistry r;
    reflection::EnumSerializer enums;
    reflection::JsonSerializer serializer(r, enums);

    r.emplace<fs::path>({"fs::path"});
    serializer.emplace<fs::path>(pathFromJson, pathToJson);

    r.emplace<fs::file_time_type>({"file_time_type"});
    serializer.emplace<fs::file_time_type>(fileTimeFromJson, fileTimeToJson);

    reflection::TypeInfoBuilder<ImportResult>("ImportResult")
        .property<&ImportResult::inputFilePath>("inputFilePath")
        .property<&ImportResult::lastWriteTime>("lastWriteTime")
        .property<&ImportResult::artifactPaths>("artifactPaths")
        .emplace(r);

    reflection::TypeInfoBuilder<AssetId>("AssetId")
        .property<&AssetId::inputFilePath>("inputFilePath")
        .property<&AssetId::artifactPath>("artifactPath")
        .emplace(r);

    ImportRegistry importers{};
    importers.emplace([](std::function<void()> const& onComplete) { std::cout << "texture import" << std::endl; },
                      {"jpg", "jpeg", "png"});
    importers.emplace([](std::function<void()> const& onComplete) { std::cout << "gltf" << std::endl; }, {"gltf"});
    AssetDatabase db(threadPool, serializer, importers, inputDirectory, loadDirectory);

    db.importFile("models/sea_house/scene.gltf");
    db.importFile("models/sea_house/scene2.gltf");
    db.importFile("models/sea_house/scene.gltf2");
    return 0;
}