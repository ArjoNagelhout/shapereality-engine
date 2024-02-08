//
// Created by Arjo Nagelhout on 07/02/2024.
//

#include <gtest/gtest.h>
#include <reflection/type_info.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/writer.h>


using namespace reflection;

namespace json_tests
{
    struct SourceData
    {
        std::string a = "This is a piece of text.";
        float b = 1.546f;
        double c = 10203.2;
        int d = 123;
        bool e = true;
    };

    void setup(TypeInfoRegistry& r)
    {
        TypeInfoBuilder<float>("float").registerType(r);
        TypeInfoBuilder<int>("int").registerType(r);
        TypeInfoBuilder<double>("double").registerType(r);
        TypeInfoBuilder<bool>("bool").registerType(r);
        TypeInfoBuilder<std::string>("string").registerType(r);

        TypeInfoBuilder<SourceData>("SourceData")
            .addProperty<&SourceData::a>("a")
            .addProperty<&SourceData::b>("b")
            .addProperty<&SourceData::c>("c")
            .addProperty<&SourceData::d>("d")
            .addProperty<&SourceData::e>("e")
            .registerType(r);
    }

    TEST(Reflection, Json)
    {
        TypeInfoRegistry r;
        setup(r);

        SourceData source;
        TypeInfo* info = r.getTypeInfo<SourceData>();
    }
}