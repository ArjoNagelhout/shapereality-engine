//
// Created by Arjo Nagelhout on 07/02/2024.
//

#include "types.h"

#include <stack>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace editor
{
    void registerTypes(TypeInfoRegistry& r)
    {
        r.registerPrimitive<float>(PrimitiveInfo{
            .name = "float"
        });

        r.registerPrimitive<int>(PrimitiveInfo{
            .name = "int"
        });

        r.registerPrimitive<bool>(PrimitiveInfo{
            .name = "bool"
        });

        r.registerPrimitive<double>(PrimitiveInfo{
            .name = "double"
        });

        r.registerPrimitive<std::string>(PrimitiveInfo{
            .name = "string"
        });

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
        struct StackFrame
        {
            std::string name;
            type_id typeId;
            bool isPrimitive = false; // cached value to avoid calling isPrimitive multiple times
            TypeInfo* typeInfo = nullptr; // cached value to avoid calling TypeInfoRegistry::get(type_id) multiple times
            size_t index = 0;

            std::any value{};
        };

        std::stack<StackFrame> stack;
        stack.emplace(StackFrame{
            .name = "root",
            .typeId = TypeIndex<Parent3>::value(),
            .value = value
        });

        while (!stack.empty())
        {
            StackFrame& top = stack.top();

            if (top.isPrimitive)
            {
                PrimitiveInfo* primitiveInfo = r.getPrimitiveInfo(top.typeId);

                // render primitive
                type_id id = top.typeId;
                if (isType<float>(id))
                {
                    auto v = std::any_cast<float>(top.value);
                    ImGui::InputFloat(top.name.c_str(), &v);
                }
                else if (isType<int>(id))
                {
                    auto v = std::any_cast<int>(top.value);
                    ImGui::InputInt(top.name.c_str(), &v);
                }
                else if (isType<std::string>(id))
                {
                    auto v = std::any_cast<std::string>(top.value);
                    ImGui::InputText(top.name.c_str(), &v);
                }
                else if (isType<bool>(id))
                {
                    auto v = std::any_cast<bool>(top.value);
                    ImGui::Checkbox(top.name.c_str(), &v);
                }
                else if (isType<double>(id))
                {
                    auto v = std::any_cast<double>(top.value);
                    ImGui::InputDouble(top.name.c_str(), &v);
                }

                stack.pop();
            }
            else
            {
                bool open = true;
                // iterate over properties and render header
                if (top.index == 0)
                {
                    top.typeInfo = r.getTypeInfo(top.typeId);

                    // begin tree node
                    open = ImGui::TreeNode((top.name + " (" + (top.typeInfo ? top.typeInfo->name : "Unregistered type") + ")").c_str());
                }

                if (open && top.typeInfo && top.index < top.typeInfo->properties.size())
                {
                    PropertyInfo& property = top.typeInfo->properties[top.index];
                    stack.emplace(StackFrame{
                        .name = property.name,
                        .typeId = property.typeId,
                        .isPrimitive = r.isPrimitive(property.typeId),
                        .value = property.getter(top.value)
                    });
                    top.index++;
                }
                else
                {
                    // end tree node
                    if (open)
                    {
                        ImGui::TreePop();
                    }
                    stack.pop();
                }
            }
        }
    }
}