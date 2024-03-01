//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "png.h"

#include <iostream>

namespace asset
{
    std::vector<Asset> importPng(std::filesystem::path const& absolutePath)
    {
        std::cout << "imported png file from " << absolutePath << std::endl;
        return {};
    }
}