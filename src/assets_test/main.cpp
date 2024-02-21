//
// Created by Arjo Nagelhout on 31/01/2024.
//

#include <asset/asset_database.h>
#include <iostream>

using namespace asset;

int main(int argc, char* argv[])
{
    fs::path inputDirectory(argv[1]);
    fs::path loadDirectory(argv[2]);

    AssetDatabase db(inputDirectory, loadDirectory);

    fs::path const inputFilePath = "models/sea_house/scene.gltf";
    std::vector<AssetId> artifacts = db.importFile(inputFilePath);

    for (auto& id : artifacts)
    {
        std::cout << id.string() << std::endl;
    }

    return 0;
}