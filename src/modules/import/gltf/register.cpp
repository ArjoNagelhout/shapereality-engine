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
            .emplace(types);
    }

    void registerImporters(asset::ImportRegistry& importRegistry)
    {
        importRegistry.emplace(importGltfNew, {"gltf"});
    }
}