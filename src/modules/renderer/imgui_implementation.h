//
// Created by Arjo Nagelhout on 20/01/2024.
//

#ifndef SHAPEREALITY_IMGUI_IMPLEMENTATION_H
#define SHAPEREALITY_IMGUI_IMPLEMENTATION_H

#include "graphics/device.h"
#include "imgui.h"
#include "graphics/command_buffer.h"

namespace renderer
{
    // imgui backend implementation against the graphics-api agnostic graphics module
    // https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md

    // At initialization:
    //   call ImGui::CreateContext()
    //   call ImGui_ImplXXXX_Init() for each backend.
    bool ImGui_ImplShapeReality_Init(graphics::IDevice* pDevice);

    // At shutdown:
    //   call ImGui_ImplXXXX_Shutdown() for each backend.
    //   call ImGui::DestroyContext()
    void ImGui_ImplShapeReality_Shutdown();

    // At the beginning of your frame:
    //   call ImGui_ImplXXXX_NewFrame() for each backend.
    //   call ImGui::NewFrame()
    void ImGui_ImplShapeReality_NewFrame(graphics::RenderPassDescriptor* pRenderPassDescriptor);

    // At the end of your frame:
    //   call ImGui::Render()
    //   call ImGui_ImplXXXX_RenderDrawData() for your Renderer backend.
    void ImGui_ImplShapeReality_RenderDrawData(ImDrawData* drawData,
                                               graphics::ICommandBuffer* pCommandBuffer);

    // called by Init/NewFrame/Shutdown
    bool ImGui_ImplShapeReality_CreateFontsTexture(graphics::IDevice* pDevice);
    void ImGui_ImplShapeReality_DestroyFontsTexture();
    bool ImGui_ImplShapeReality_CreateDeviceObjects(graphics::IDevice* pDevice);
    void ImGui_ImplShapeReality_DestroyDeviceObjects();
}

#endif //SHAPEREALITY_IMGUI_IMPLEMENTATION_H
