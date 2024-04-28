//
// Created by Arjo Nagelhout on 26/04/2024.
//

#ifndef SHAPEREALITY_AXES_H
#define SHAPEREALITY_AXES_H

#include <asset/asset_handle.h>
#include <asset/asset_database.h>

namespace editor
{
    asset::Asset createAxesMesh(asset::AssetDatabase& assets, asset::AssetId id);
}

#endif //SHAPEREALITY_AXES_H
