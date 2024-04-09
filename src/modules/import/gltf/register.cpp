//
// Created by Arjo Nagelhout on 09/04/2024.
//

#include "register.h"

#include "gltf.h"

#include <reflection/class.h>

namespace import_::gltf
{
    REGISTER_REFLECTION
    {
        reflection::ClassInfoBuilder<GltfImportParameters>("GltfImportParameters")
            .emplace(reflection.types);
    }

    REGISTER_IMPORTERS
    {
        importers.emplace(importGltfNew, {"gltf"});
    }
}