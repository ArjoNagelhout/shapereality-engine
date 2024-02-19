//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_ASSET_DATABASE_H
#define SHAPEREALITY_ASSET_DATABASE_H

#include <string>

namespace asset
{
    class AssetDatabase
    {
    public:
        explicit AssetDatabase();

        ~AssetDatabase();

        // adds a source directory to the search path
        void addSourceDirectory();

        void loadAsset(std::string const& identifier);

    private:

    };
}

#endif //SHAPEREALITY_ASSET_DATABASE_H
