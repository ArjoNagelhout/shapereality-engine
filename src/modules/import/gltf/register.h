//
// Created by Arjo Nagelhout on 09/04/2024.
//

#ifndef SHAPEREALITY_REGISTER_IMPORT_GLTF_H
#define SHAPEREALITY_REGISTER_IMPORT_GLTF_H

#include <asset/import_registry.h>

#include <reflection/reflection.h>

namespace import_::gltf
{
    // register reflection and importer

    void register_(reflection::Reflection& reflection);

    void register_(asset::ImportRegistry& importers);
}

#endif //SHAPEREALITY_REGISTER_IMPORT_GLTF_H
