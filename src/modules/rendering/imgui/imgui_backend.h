//
// Created by Arjo Nagelhout on 20/01/2024.
//

#ifndef SHAPEREALITY_IMGUI_BACKEND_H
#define SHAPEREALITY_IMGUI_BACKEND_H

#include "graphics/device.h"
#include "imgui.h"
#include "graphics/command_buffer.h"
#include "graphics/input.h"

namespace rendering::imgui_backend
{
    // imgui backend implementation against the graphics-api agnostic graphics module
    // https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md

    // should be called in onEvent
    void onEvent(graphics::InputEvent const& event);

    // At initialization:
    //   call ImGui::CreateContext()
    //   call ImGui_ImplXXXX_Init() for each backend.
    bool init(graphics::IDevice* device, graphics::Window* window, graphics::IShaderLibrary* shaderLibrary);

    // At shutdown:
    //   call ImGui_ImplXXXX_Shutdown() for each backend.
    //   call ImGui::DestroyContext()
    void shutdown();

    // At the beginning of your frame:
    //   call ImGui_ImplXXXX_NewFrame() for each backend.
    //   call ImGui::NewFrame()
    void newFrame(graphics::RenderPassDescriptor const& renderPassDescriptor);

    // sets window size and updates mouse data
    // gets called by newFrame()
    void updateIO();

    // At the end of your frame:
    //   call ImGui::Render()
    //   call ImGui_ImplXXXX_RenderDrawData() for your Renderer backend.
    void renderDrawData(ImDrawData* drawData, graphics::ICommandBuffer* commandBuffer);

    // called by Init/NewFrame/Shutdown
    bool createFontsTexture(graphics::IDevice* device);
    void destroyFontsTexture();
    bool createDeviceObjects(graphics::IDevice* device);
    void destroyDeviceObjects();
}

#endif //SHAPEREALITY_IMGUI_BACKEND_H
