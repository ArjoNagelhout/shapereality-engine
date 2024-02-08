//
// Created by Arjo Nagelhout on 07/02/2024.
//

#include "types.h"

#include <stack>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

#include <nlohmann/json.hpp>

namespace editor
{
    void registerTypes(TypeInfoRegistry& r)
    {
        TypeInfoBuilder<float>("float").registerType(r);
        TypeInfoBuilder<int>("int").registerType(r);
        TypeInfoBuilder<double>("double").registerType(r);
        TypeInfoBuilder<bool>("bool").registerType(r);
        TypeInfoBuilder<std::string>("string").registerType(r);

        TypeInfoBuilder<Child>("Child")
            .addProperty<&Child::x>("x")
            .addProperty<&Child::y>("y")
            .addProperty<&Child::z>("z")
            .registerType(r);

        TypeInfoBuilder<Child2>("Child2")
            .addProperty<&Child2::something>("something")
            .addProperty<&Child2::yourValue>("yourValue")
            .addProperty<&Child2::weValue>("weValue")
            .addProperty<&Child2::veryImportant>("veryImportant")
            .registerType(r);

        TypeInfoBuilder<Parent1>("Parent1")
            .addProperty<&Parent1::c1>("c1")
            .addProperty<&Parent1::c2>("c2")
            .addProperty<&Parent1::c3>("c3")
            .registerType(r);

        TypeInfoBuilder<Parent2>("Parent2")
            .addProperty<&Parent2::p1>("p1")
            .addProperty<&Parent2::p2>("p2")
            .addProperty<&Parent2::w>("w")
            .registerType(r);

        TypeInfoBuilder<Parent3>("Parent3")
            .addProperty<&Parent3::p1>("p1")
            .addProperty<&Parent3::p2>("p2")
            .addProperty<&Parent3::p3>("p3")
            .addProperty<&Parent3::p4>("p4")
            .addProperty<&Parent3::something>("something")
            .addProperty<&Parent3::aValue>("aValue")
            .addProperty<&Parent3::someValue>("someValue")
            .addProperty<&Parent3::myValue>("myValue")
            .addProperty<&Parent3::ourValue>("ourValue")
            .addProperty<&Parent3::c1>("c1")
            .addProperty<&Parent3::c2>("c2")
            .addProperty<&Parent3::c3>("c3")
            .addProperty<&Parent3::w>("w")
            .registerType(r);
    }

    void render(TypeInfoRegistry& r, Parent3& value)
    {
        iterateUsingStack<Parent3>(r, "root", &value,
                                   [](StackFrame const& f) -> bool {

                                       std::string label = f.name + " (" + f.typeInfo->name + ")";

                                       if (!f.typeInfo)
                                       {
                                           return false;
                                       }

                                       type_id id = f.typeId;
                                       std::any value = f.value;

                                       if (!f.typeInfo->properties.empty())
                                       {
                                           return ImGui::TreeNode((f.name + " (" + f.typeInfo->name + ")").c_str());
                                       }
                                       else if (isType<float>(id))
                                       {
                                           auto* v = std::any_cast<float*>(value);
                                           ImGui::InputFloat(label.c_str(), v);
                                       }
                                       else if (isType<int>(id))
                                       {
                                           auto* v = std::any_cast<int*>(value);
                                           ImGui::InputInt(label.c_str(), v);
                                       }
                                       else if (isType<std::string>(id))
                                       {
                                           auto* v = std::any_cast<std::string*>(value);
                                           ImGui::InputText(label.c_str(), v);
                                       }
                                       else if (isType<bool>(id))
                                       {
                                           auto* v = std::any_cast<bool*>(value);
                                           ImGui::Checkbox(label.c_str(), v);
                                       }
                                       else if (isType<double>(id))
                                       {
                                           auto* v = std::any_cast<double*>(value);
                                           ImGui::InputDouble(label.c_str(), v);
                                       }
                                       return false;
                                   },
                                   [](StackFrame const& f) {
                                       ImGui::TreePop();
                                   });
    }
    
    [[nodiscard]] std::string toJson(TypeInfoRegistry& r, Parent3& value)
    {
        return {};
    }

    [[nodiscard]] Parent3 fromJson(TypeInfoRegistry& r, std::string const& json)
    {
        return Parent3{};
    }
}