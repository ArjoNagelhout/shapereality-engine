//
// Created by Arjo Nagelhout on 12/04/2024.
//

#include "register.h"

#include <texture/png.h>

namespace import_::texture
{
    void register_(asset::ImportRegistry& importers)
    {
        importers.emplace(importPng, {"png"});
    }
}