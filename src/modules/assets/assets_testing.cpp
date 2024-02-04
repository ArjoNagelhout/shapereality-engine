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
    fs::path sourceDirectory(argv[1]);
    fs::path cacheDirectory;

    TypeInfoRegistry r;
//    TypeInfo info = TypeInfoBuilder("MeshImportSettings")
//        .addProperty<bool>("doSomething")
//        .addProperty<bool>("somethingElse")
//        .addProperty<float>("someValue")
//        .addProperty<int>("otherValue")
//        .build();
//
//    r.add<MeshImportSettings>(info);

//    std::string name = "someValue";
    //size_t offset = offsetof(MeshImportSettings, name);

    SourceDirectory src(sourceDirectory, cacheDirectory);

    return 0;
}