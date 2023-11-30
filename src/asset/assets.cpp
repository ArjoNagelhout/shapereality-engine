//
// Created by Arjo Nagelhout on 07/11/2023.
//

#include "assets.h"

void Asset::load() {

}

void Asset::unload() {

}

bool Asset::isLoaded() const {
    return false;
}

std::filesystem::path Asset::getMetadataPath() const {
    return {};
}

Asset::~Asset() {

}

Asset::Asset() {

}

void Asset::saveMetadata() {

}

Project::Project(const std::filesystem::path &projectDirectory) {

}
