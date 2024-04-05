//
// Created by Arjo Nagelhout on 29/01/2024.
//

#include "ui.h"

#include <imgui.h>
#include <renderer/imgui/imgui_backend.h>
#include <entity/components/hierarchy.h>
#include <renderer/transform.h>
#include <renderer/scene_renderer.h>

#include <math/matrix.h>
#include <math/matrix.inl>

#include <math/quaternion.h>
#include <math/quaternion.inl>

#include <scene/scene.h>

using namespace entity;
using namespace renderer;
using namespace scene;
using namespace reflection;

namespace editor
{
    void setupImGuiStyle()
    {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
        colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
        colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
//        colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
//        colors[ImGuiCol_DockingEmptyBg]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding                     = ImVec2(8.00f, 8.00f);
        style.FramePadding                      = ImVec2(3.00f, 2.00f);
        style.CellPadding                       = ImVec2(6.00f, 6.00f);
        style.ItemSpacing                       = ImVec2(6.00f, 2.00f);
        style.ItemInnerSpacing                  = ImVec2(5.00f, 6.00f);
        style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
        style.IndentSpacing                     = 25;
        style.ScrollbarSize                     = 11;
        style.GrabMinSize                       = 7;
        style.WindowBorderSize                  = 0;
        style.ChildBorderSize                   = 0;
        style.PopupBorderSize                   = 0;
        style.FrameBorderSize                   = 0;
        style.TabBorderSize                     = 0;
        style.WindowRounding                    = 6;
        style.ChildRounding                     = 5;
        style.FrameRounding                     = 4;
        style.PopupRounding                     = 4;
        style.ScrollbarRounding                 = 12;
        style.GrabRounding                      = 2;
        style.LogSliderDeadzone                 = 4;
        style.TabRounding                       = 3;
    }

    UI::UI(graphics::IDevice* device, graphics::Window* window, graphics::IShaderLibrary* shaderLibrary)
    {
        // create imgui context
        ImGui::CreateContext();
        imgui_backend::init(device, window, shaderLibrary);
        setupImGuiStyle();
    }

    UI::~UI()
    {
        imgui_backend::shutdown();
        ImGui::DestroyContext();
    }

    void UI::onEvent(graphics::InputEvent const& event)
    {
        imgui_backend::onEvent(event);
    }

    void UI::update(graphics::RenderPassDescriptor const& renderPassDescriptor)
    {
        imgui_backend::newFrame(renderPassDescriptor);
        ImGui::NewFrame();

        bool openImGuiDemoWindow = true;
        ImGui::ShowDemoWindow(&openImGuiDemoWindow);

        // simple scene representation
        if (ImGui::Begin("Scene"))
        {
            if (ImGui::Button("New object"))
            {

            }

            // iterate over all entities inside the transform hierarchy
            for (auto [entityId, hierarchy, transform]: r->view<entity::HierarchyComponent, TransformComponent>())
            {
                std::string const id = std::to_string(entityId);
                bool selected = selectedEntityId == entityId;

                // selectable
                if (ImGui::Selectable(("##selectable_" + id).c_str(), &selected,
                                      ImGuiSelectableFlags_AllowItemOverlap,
                                      ImVec2(0.f, 20.f)))
                {
                    selectedEntityId = selected ? entityId : kNullEntityId;
                }

                // visible checkbox
                ImGui::SameLine();
                bool isVisible = r->entityContainsComponent<VisibleComponent>(entityId);
                if (ImGui::Checkbox(("##visible_" + id).c_str(), &isVisible))
                {
                    isVisible ? r->addComponent<VisibleComponent>(entityId)
                              : r->removeComponent<VisibleComponent>(entityId);
                }

                // entity name
                ImGui::SameLine();
                ImGui::TextUnformatted(("entityId: " + id).c_str());
            }
        }
        ImGui::End();

        if (ImGui::Begin("Inspector"))
        {
            if (selectedEntityId != kNullEntityId)
            {
                // display entity
                bool isVisible = r->entityContainsComponent<VisibleComponent>(selectedEntityId);

                if (ImGui::Checkbox("Visible", &isVisible))
                {
                    isVisible ? r->addComponent<VisibleComponent>(selectedEntityId)
                              : r->removeComponent<VisibleComponent>(selectedEntityId);
                }

                auto& transform = r->getComponent<TransformComponent>(selectedEntityId);

                if (ImGui::InputFloat3("Local Position", &transform.localPosition[0]))
                {
                    setLocalPosition(*r, selectedEntityId, transform.localPosition);
                }

                math::Vector3 eulerInDegrees = transform.localRotation.toEulerInDegrees();
                if (ImGui::InputFloat3("Local Rotation", &eulerInDegrees[0]))
                {
                    setLocalRotation(*r, selectedEntityId, math::Quaternion::createFromEulerInDegrees(eulerInDegrees));
                }

                if (ImGui::InputFloat3("Local Scale", &transform.localScale[0]))
                {
                    setLocalScale(*r, selectedEntityId, transform.localScale);
                }
            }
        }
        ImGui::End();

        ImGuiIO& io = ImGui::GetIO();

        capturedKeyboard = io.WantCaptureKeyboard;
        capturedMouse = io.WantCaptureMouse;
    }

    void UI::render(graphics::ICommandBuffer* commandBuffer)
    {
        // ImGui: Render
        ImGui::Render();
        imgui_backend::renderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

    bool UI::getCapturedMouse() const
    {
        return capturedMouse;
    }

    bool UI::getCapturedKeyboard() const
    {
        return capturedKeyboard;
    }

    void UI::setRegistry(entity::EntityRegistry* _r)
    {
        r = _r;
    }
}