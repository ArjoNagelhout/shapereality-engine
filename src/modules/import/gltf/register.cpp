//
// Created by Arjo Nagelhout on 09/04/2024.
//

#include "register.h"

#include "gltf.h"

#include <reflection/class.h>

namespace import_::gltf
{
    void register_(reflection::Reflection& reflection)
    {
        reflection::register_::Class<GltfImportParameters>("GltfImportParameters")
            .member<&GltfImportParameters::vertexAttributesToImport>("vertexAttributesToImport")
            .emplace(reflection.types);
    }

    void register_(asset::ImportRegistry& importers)
    {
        importers.emplace(importGltf, {"gltf"});
    }
}