//
// Created by Arjo Nagelhout on 20/01/2024.
//

#include "imgui_implementation.h"

#include "graphics/render_pipeline_state.h"
#include "graphics/texture.h"
#include "graphics/render_pass.h"

namespace renderer
{
    struct FramebufferDescriptor
    {
        unsigned long sampleCount;
        graphics::PixelFormat colorPixelFormat;
        graphics::PixelFormat depthPixelFormat;
        graphics::PixelFormat stencilPixelFormat;

        explicit FramebufferDescriptor(graphics::RenderPassDescriptor const& renderPassDescriptor)
        {
            //sampleCount = renderPassDescriptor.colorAttachments[0].pTexture.get()
        }
    };

    struct ImGui_ImplShapeReality_Data
    {
        graphics::IDevice* pDevice;
        std::unique_ptr<graphics::IDepthStencilState> pDepthStencilState;
        std::unique_ptr<graphics::ITexture> pFontTexture;
        std::unique_ptr<FramebufferDescriptor> pFramebufferDescriptor;
//        std::unordered_map<

        ImGui_ImplShapeReality_Data() {}
    };

    static ImGui_ImplShapeReality_Data* ImGui_ImplShapeReality_GetBackendData()
    {
        if (ImGui::GetCurrentContext())
        {
            return (ImGui_ImplShapeReality_Data*)ImGui::GetIO().BackendRendererUserData;
        }
        else
        {
            return nullptr;
        }
    }

    bool ImGui_ImplShapeReality_Init(graphics::IDevice* pDevice)
    {
        auto* bd = new ImGui_ImplShapeReality_Data();
        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererUserData = (void*)bd;
        io.BackendRendererName = "imgui_impl_shapereality";
        //io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

        bd->pDevice = pDevice;

        return true;
    }

    void ImGui_ImplShapeReality_Shutdown()
    {
        ImGui_ImplShapeReality_Data* bd = ImGui_ImplShapeReality_GetBackendData();
        IM_ASSERT(bd != nullptr && "No renderer backend to shutdown, or already shut down?");
        ImGui_ImplShapeReality_DestroyDeviceObjects();
        delete bd;

        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererName = nullptr;
        io.BackendRendererUserData = nullptr;
//        io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;
    }

    void ImGui_ImplShapeReality_NewFrame(graphics::RenderPassDescriptor const& renderPassDescriptor)
    {
        ImGui_ImplShapeReality_Data* bd = ImGui_ImplShapeReality_GetBackendData();
        IM_ASSERT(bd != nullptr && "No ShapeReality context. Did you call ImGui_ImplShapeReality_Init() ?");
//        bd.frame

        if (!bd->pDepthStencilState)
        {
            ImGui_ImplShapeReality_CreateDeviceObjects(bd->pDevice);
        }
    }

    void ImGui_ImplShapeReality_RenderDrawData(ImDrawData* drawData,
                                               graphics::ICommandBuffer* pCommandBuffer)
    {
        ImGui_ImplShapeReality_Data* bd = ImGui_ImplShapeReality_GetBackendData();

        // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
        int fb_width = (int)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
        int fb_height = (int)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
        if (fb_width <= 0 || fb_height <= 0 || drawData->CmdListsCount == 0)
            return;

        // Try to retrieve a render pipeline state that is compatible with the framebuffer config for this frame
        // The hit rate for this cache should be very near 100%.
//        id<MTLRenderPipelineState> renderPipelineState = ctx.renderPipelineStateCache[ctx.framebufferDescriptor];
//        if (renderPipelineState == nil)
//        {
//            // No luck; make a new render pipeline state
//            renderPipelineState = [ctx renderPipelineStateForFramebufferDescriptor:ctx.framebufferDescriptor device:commandBuffer.device];
//
//            // Cache render pipeline state for later reuse
//            ctx.renderPipelineStateCache[ctx.framebufferDescriptor] = renderPipelineState;
//        }
    }

    bool ImGui_ImplShapeReality_CreateFontsTexture(graphics::IDevice* pDevice)
    {
        return false;
    }

    void ImGui_ImplShapeReality_DestroyFontsTexture()
    {

    }

    bool ImGui_ImplShapeReality_CreateDeviceObjects(graphics::IDevice* pDevice)
    {
        return false;
    }

    void ImGui_ImplShapeReality_DestroyDeviceObjects()
    {

    }
}