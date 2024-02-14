//
// Created by Arjo Nagelhout on 11/02/2024.
//

#include <gtest/gtest.h>

#include <reflection/type_info.h>
#include <reflection/json.h>

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
        Registry r;
        add<int>(r, {.name = "int"});
        add<float>(r, {.name = "float"});
        add<bool>(r, {.name = "bool"});
        add<double>(r, {.name = "double"});
        add<std::string>(r, {.name = "string"});

        TypeInfo data3Info{.name = "Data3"};
        property<Data3, &Data3::a>(data3Info, "a");
        property<Data3, &Data3::b>(data3Info, "b");
        property<Data3, &Data3::c>(data3Info, "c");
        property<Data3, &Data3::d>(data3Info, "d");
        property<Data3, &Data3::e>(data3Info, "e");
        add<Data3>(r, std::move(data3Info));

        TypeInfo dataInfo{.name = "Data"};
        property<Data, &Data::data>(dataInfo, "data");
        property<Data, &Data::silly>(dataInfo, "silly");
        add<Data>(r, std::move(dataInfo));

        TypeInfo data2Info{.name = "Data2"};
        property<Data2, &Data2::myValues>(data2Info, "myValues");
        property<Data2, &Data2::data3s>(data2Info, "data3s");
        add<Data2>(r, std::move(data2Info));

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

        nlohmann::json out = toJson<Data>(r, data);
        std::cout << out.dump(2) << std::endl;

        std::string in = R"({"data":[{"data3s":{},"myValues":{"second":[1.2000000476837158,1.2999999523162842],"something":[0.10000000149011612,0.20000000298023224,0.30000001192092896],"third":[1.0]}},{"data3s":{"asdflkajsdf":{"a":1.0,"b":false,"c":1346,"d":1.6,"e":"yes yes"},"beng":{"a":1.2999999523162842,"b":false,"c":1346,"d":1.6,"e":"no no no"},"oezoe":{"a":1.2999999523162842,"b":true,"c":1346,"d":1.6,"e":"yes yes"},"owoe":{"a":1.2999999523162842,"b":false,"c":12342384,"d":1.6,"e":"yes yes"}},"myValues":{"wow":[1.0]}}],"silly":[]})";
        Data result = fromJson<Data>(r, in);
    }
}