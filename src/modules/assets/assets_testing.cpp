//
// Created by Arjo Nagelhout on 31/01/2024.
//

#include "source_directory.h"

#include <iostream>

namespace fs = std::filesystem;
using namespace assets;

#include "nlohmann/json.hpp"

using namespace nlohmann;

#include "reflection/type_info.h"

using namespace reflection;

struct MeshImportSettings
{
    bool doSomething = false;
    bool somethingElse = true;
    float someValue = 1324.0f;
    int otherValue = 98;

    json serialize()
    {
        json j = json::object();
        j["doSomething"] = doSomething;
        j["somethingElse"] = somethingElse;
        j["someValue"] = someValue;
        j["otherValue"] = otherValue;
        return j;
    }

    void deserialize(std::string const& json)
    {
        auto j = json::parse(json);
        doSomething = j["doSomething"];
        somethingElse = j["somethingElse"];
        someValue = j["someValue"];
        otherValue = j["otherValue"];
    }
};

int main(int argc, char* argv[])
{
    fs::path sourceDirectoryPath(argv[1]);
    fs::path cacheDirectoryPath;

    SourceDirectory src(sourceDirectoryPath, cacheDirectoryPath);

    TypeInfoRegistry r;

    PrimitiveInfo string = PrimitiveInfo{
        .name = "std::string"
    };
    r.registerPrimitive<std::string>(std::move(string));

    TypeInfo info = TypeInfoBuilder<MeshImportSettings>("MeshImportSettings")
        .addProperty<&MeshImportSettings::doSomething>("doSomething")
        .addProperty<&MeshImportSettings::somethingElse>("somethingElse")
        .addProperty<&MeshImportSettings::someValue>("someValue")
        .addProperty<&MeshImportSettings::otherValue>("otherValue")
        .build();
    r.registerType<MeshImportSettings>(std::move(info));

    TypeInfo* storedInfo = r.get<MeshImportSettings>();

    for (auto& property: storedInfo->properties)
    {
        std::cout << property.name << std::endl;
    }

    // what is the usage pattern:
    // 1. serialization and deserialization of data to disk
    //    via json
    // 2. in editor reflection of data types so that we can edit them in ImGui
    //    or our custom UI.

    // for 2, we need to iterate over the given data type, e.g. the TypeInfo for MeshImportSettings,
    // and recursively create sections for properties that are not primitive types.

    // what types of primitive types are there and what do they need?
    // Matrix<Rows, Columns> [0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3]
    // Vector<Size> [0, 1, 2, 3]
    // float 1.2
    // int 1
    // bool false
    // double 1.4
    // std::string "something along the lines of"
    // Asset<AssetType> <ASSET_ID_HERE>
    // entity_id <ENTITY_ID_HERE>

    // how do we register primitive types?

    return 0;
}