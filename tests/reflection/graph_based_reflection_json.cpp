//
// Created by Arjo Nagelhout on 11/02/2024.
//

#include <gtest/gtest.h>

#include <reflection/type_info.h>
#include "reflection/serialization/json.h"

using namespace reflection;

namespace graph_based_reflection_json
{
    struct Data3
    {
        float a = 1.3f;
        bool b = false;
        int c = 1346;
        double d = 1.6;
        std::string e = "yes yes";
    };

    struct Data2
    {
        std::unordered_map<std::string, std::vector<float>> myValues;

        std::unordered_map<std::string, Data3> data3s;
    };

    struct Data
    {
        std::vector<Data2> data;
        std::vector<std::vector<std::vector<std::unordered_map<int, std::vector<std::vector<float>>>>>> silly;
    };

    TEST(Reflection, JsonSerialization)
    {
        TypeInfoRegistry r;

        TypeInfoBuilder<Data3>("Data3")
            .property<&Data3::a>("a")
            .property<&Data3::b>("b")
            .property<&Data3::c>("c")
            .property<&Data3::d>("d")
            .property<&Data3::e>("e")
            .emplace(r);

        TypeInfoBuilder<Data>("Data")
            .property<&Data::data>("data")
            .property<&Data::silly>("silly")
            .emplace(r);

        TypeInfoBuilder<Data2>("Data2")
            .property<&Data2::myValues>("myValues")
            .property<&Data2::data3s>("data3s")
            .emplace(r);

        json::Converter converter;

        Data data{
            .data = {
                Data2{
                    .myValues{
                        {"something", {0.1f, 0.2f, 0.3f}},
                        {"second", {1.2f, 1.3f}},
                        {"third", {1.0f}}
                    }
                },
                Data2{
                    .myValues{
                        {"wow", {1.0f}}
                    },
                    .data3s{
                        {
                            "asdflkajsdf", Data3{
                            .a = 1.f
                        }},
                        {
                            "oezoe", Data3{
                            .b = true
                        }},
                        {
                            "owoe", Data3{
                            .c = 12342384
                        }},
                        {
                            "beng", Data3{
                            .e = "no no no"
                        }}
                    }
                }
            }
        };

        nlohmann::json out = json::toJson<Data>(r, converter, data);
        std::cout << out.dump(2) << std::endl;

        std::string in = R"({"data":[{"data3s":{},"myValues":{"second":[1.2000000476837158,1.2999999523162842],"something":[0.10000000149011612,0.20000000298023224,0.30000001192092896],"third":[1.0]}},{"data3s":{"asdflkajsdf":{"a":1.0,"b":false,"c":1346,"d":1.6,"e":"yes yes"},"beng":{"a":1.2999999523162842,"b":false,"c":1346,"d":1.6,"e":"no no no"},"oezoe":{"a":1.2999999523162842,"b":true,"c":1346,"d":1.6,"e":"yes yes"},"owoe":{"a":1.2999999523162842,"b":false,"c":12342384,"d":1.6,"e":"yes yes"}},"myValues":{"wow":[1.0]}}],"silly":[]})";
        Data result = json::fromJson<Data>(r, converter, in);
    }
}