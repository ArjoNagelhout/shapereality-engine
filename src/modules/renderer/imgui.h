//
// Created by Arjo Nagelhout on 20/01/2024.
//

#ifndef SHAPEREALITY_IMGUI_H
#define SHAPEREALITY_IMGUI_H

namespace renderer
{
    // imgui backend implementation against the graphics-api agnostic graphics module
    // https://github.com/ocornut/imgui/blob/master/docs/EXAMPLES.md

    // At initialization:
    //   call ImGui::CreateContext()
    //   call ImGui_ImplXXXX_Init() for each backend.
    void ImGui_ImplShapeReality_Init();

    // At the beginning of your frame:
    //   call ImGui_ImplXXXX_NewFrame() for each backend.
    //   call ImGui::NewFrame()
    void ImGui_ImplShapeReality_NewFrame();

    // At the end of your frame:
    //   call ImGui::Render()
    //   call ImGui_ImplXXXX_RenderDrawData() for your Renderer backend.
    void ImGui_ImplShapeReality_RenderDrawData();

    // At shutdown:
    //   call ImGui_ImplXXXX_Shutdown() for each backend.
    //   call ImGui::DestroyContext()
    void ImGui_ImplShapeReality_Shutdown();
}

#endif //SHAPEREALITY_IMGUI_H
