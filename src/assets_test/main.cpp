//
// Created by Arjo Nagelhout on 31/01/2024.
//

#include <assets/source_directory.h>
#include <assets/importer_registry.h>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    // we want to have some way of identifying which assets are being referenced inside a scene
    // so that we can load them on runtime.

    fs::path sourceDirectoryPath(argv[1]);
    fs::path cacheDirectoryPath(argv[1]);
    cacheDirectoryPath.append(".cache");

//    assets::ImporterRegistry r;
//    r.emplace({".", ".txt"}, [](std::string const& source, std::string const& target){
//        std::cout << "importing text file" << std::endl;
//    });
//    r.emplace({".fbx"}, [](std::string const& source, std::string const& target){
//        std::cout << "importing fbx file" << std::endl;
//    });

    assets::SourceDirectory src(sourceDirectoryPath, cacheDirectoryPath);

    return 0;
}