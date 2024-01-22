//
// Created by Arjo Nagelhout on 20/01/2024.
//

#ifndef SHAPEREALITY_IMGUI_IMPLEMENTATION_H
#define SHAPEREALITY_IMGUI_IMPLEMENTATION_H

#include "graphics/device.h"
#include "imgui.h"
#include "graphics/command_buffer.h"

namespace renderer::imgui_backend
{
    // imgui backend implementation against the graphics-api agnostic graphics module
    // https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md

    // At initialization:
    //   call ImGui::CreateContext()
    //   call ImGui_ImplXXXX_Init() for each backend.
    bool init(graphics::IDevice* pDevice, graphics::IShaderLibrary* pShaderLibrary);

    // At shutdown:
    //   call ImGui_ImplXXXX_Shutdown() for each backend.
    //   call ImGui::DestroyContext()
    void shutdown();

    // At the beginning of your frame:
    //   call ImGui_ImplXXXX_NewFrame() for each backend.
    //   call ImGui::NewFrame()
    void newFrame(graphics::RenderPassDescriptor const& renderPassDescriptor);

    // At the end of your frame:
    //   call ImGui::Render()
    //   call ImGui_ImplXXXX_RenderDrawData() for your Renderer backend.
    void renderDrawData(ImDrawData* drawData, graphics::ICommandBuffer* pCommandBuffer);

    // called by Init/NewFrame/Shutdown
    bool createFontsTexture(graphics::IDevice* pDevice);
    void destroyFontsTexture();
    bool createDeviceObjects(graphics::IDevice* pDevice);
    void destroyDeviceObjects();
}

#endif //SHAPEREALITY_IMGUI_IMPLEMENTATION_H
