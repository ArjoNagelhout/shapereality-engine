//
// Created by Arjo Nagelhout on 29/01/2024.
//

#include "editor_ui.h"

#include "imgui.h"
#include "renderer/imgui_backend.h"
#include "entity/components/hierarchy.h"
#include "renderer/transform.h"
#include "renderer/scene.h"

#include "math/matrix.h"
#include "math/matrix.inl"

#include "math/quaternion.h"
#include "math/quaternion.inl"

using namespace entity;
using namespace renderer;

namespace editor
{
    UI::UI(graphics::IDevice* device, graphics::Window* window, graphics::IShaderLibrary* shaderLibrary)
    {
        // create imgui context
        ImGui::CreateContext();
        imgui_backend::init(device, window, shaderLibrary);
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
                    selectedEntityId = selected ? entityId : TOMBSTONE;
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
            if (selectedEntityId != TOMBSTONE)
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

    void UI::setRegistry(entity::Registry* _r)
    {
        r = _r;
    }
}