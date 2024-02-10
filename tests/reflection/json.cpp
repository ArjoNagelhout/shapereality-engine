//
// Created by Arjo Nagelhout on 07/02/2024.
//

#include <gtest/gtest.h>
#include <reflection/type_info.h>

#include <nlohmann/json.hpp>

#include <stack>

using namespace reflection;
using namespace nlohmann;

namespace json_tests
{
    struct SubData
    {
        std::string a = "Another piece of text";
        float b = 1.1f;
        double c = 1000023.12341;
        int d = 10;
        bool e = false;
    };

    struct SourceData
    {
        std::string a = "This is a piece of text.";
        float b = 1.546f;
        double c = 10203.2;
        int d = 123;
        bool e = true;
        SubData f;
    };

    void setup(TypeInfoRegistry& r)
    {
        TypeInfoBuilder<float>("float").registerType(r);
        TypeInfoBuilder<int>("int").registerType(r);
        TypeInfoBuilder<double>("double").registerType(r);
        TypeInfoBuilder<bool>("bool").registerType(r);
        TypeInfoBuilder<std::string>("string").registerType(r);

        TypeInfoBuilder<SubData>("SubData")
            .addProperty<&SubData::a>("subdata_string_a")
            .addProperty<&SubData::b>("subdata_float_b")
            .addProperty<&SubData::c>("subdata_double_c")
            .addProperty<&SubData::d>("subdata_d")
            .addProperty<&SubData::e>("e")
            .registerType(r);

        TypeInfoBuilder<SourceData>("SourceData")
            .addProperty<&SourceData::a>("a")
            .addProperty<&SourceData::b>("b")
            .addProperty<&SourceData::c>("c")
            .addProperty<&SourceData::d>("d")
            .addProperty<&SourceData::e>("e")
            .addProperty<&SourceData::f>("f")
            .registerType(r);
    }

    TEST(Reflection, Json)
    {
        TypeInfoRegistry r;
        setup(r);

        SourceData source;

        json output;
        std::stack<json*> stack;
        stack.emplace(&output);

        auto callback = [&stack](StackFrame const& f) -> bool {
            type_id id = f.object.typeId;
            std::any value = f.value;

            json& top = *stack.top();

            if (!f.object.typeInfo->properties.empty())
            {
                top[f.name] = json{};
            }
            else if (isType<float>(id))
            {
                auto* v = std::any_cast<float*>(value);
                top[f.name] = *v;
            }
            else if (isType<int>(id))
            {
                auto* v = std::any_cast<int*>(value);
                top[f.name] = *v;
            }
            else if (isType<std::string>(id))
            {
                auto* v = std::any_cast<std::string*>(value);
                top[f.name] = *v;
            }
            else if (isType<bool>(id))
            {
                auto* v = std::any_cast<bool*>(value);
                top[f.name] = *v;
            }
            else if (isType<double>(id))
            {
                auto* v = std::any_cast<double*>(value);
                top[f.name] = *v;
            }
            else
            {
                return false;
            }

            stack.emplace(&top[f.name]);
            return true;
        };

        auto onPop = [&stack](StackFrame const& f) {
            stack.pop();
        };

        iterateUsingStack<SourceData>(r, "json_test", &source, callback, onPop);

        std::string a = output.dump(4, ' ');
        std::cout << a << std::endl;
    }
}