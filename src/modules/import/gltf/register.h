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

    REGISTER_REFLECTION;

    REGISTER_IMPORTERS;
}

#endif //SHAPEREALITY_REGISTER_IMPORT_GLTF_H
