//
// Created by Arjo Nagelhout on 06/02/2024.
//

#include <reflection/type_info.h>
#include <gtest/gtest.h>

using namespace reflection;

namespace type_info_tests
{
    struct SimpleData
    {
        float value1;
        int value2;
        bool value3;
    };

    TEST(Reflection, GetSet)
    {
        SimpleData d1{
            .value1 = 3.0f,
            .value2 = 32,
            .value3 = true,
        };

        SimpleData d2{
            .value1 = 123.0f,
            .value2 = 10,
            .value3 = false
        };

        TypeInfoRegistry r;

        TypeInfoBuilder<SimpleData>("SimpleData")
            .addProperty<&SimpleData::value1>("value1")
            .addProperty<&SimpleData::value2>("value2")
            .addProperty<&SimpleData::value3>("value3")
            .registerType(r);

        TypeInfo* info = r.getTypeInfo<SimpleData>();
        auto& prop = info->properties[0];

        auto* d1_value1 = prop.object.get<float>(&d1);
        auto* d2_value1 = prop.object.get<float>(&d2);

        ASSERT_EQ(*d1_value1, 3.0f);
        ASSERT_EQ(*d2_value1, 123.0f);

        prop.object.set(&d1, 100.0f);
        prop.object.set(&d2, 3252.0f);

        ASSERT_EQ(*d1_value1, 100.0f);
        ASSERT_EQ(*d2_value1, 3252.0f);
    }
}