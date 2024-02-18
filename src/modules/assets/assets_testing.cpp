//
// Created by Arjo Nagelhout on 31/01/2024.
//

#include "source_directory.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    fs::path sourceDirectoryPath(argv[1]);
    fs::path cacheDirectoryPath;

    assets::SourceDirectory src(sourceDirectoryPath, cacheDirectoryPath);

    return 0;
}