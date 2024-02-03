//
// Created by Arjo Nagelhout on 31/01/2024.
//

#include "assets_source_directory.h"

#include <iostream>

namespace fs = std::filesystem;
using namespace assets;

int main(int argc, char* argv[])
{
    fs::path sourceDirectory(argv[1]);
    fs::path cacheDirectory;

//    std::cout << sourceDirectory.filename() << std::endl;

    SourceAssetsDirectory src(sourceDirectory, cacheDirectory);

    return 0;
}