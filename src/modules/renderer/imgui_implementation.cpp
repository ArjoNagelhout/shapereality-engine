//
// Created by Arjo Nagelhout on 20/01/2024.
//

#include "imgui_implementation.h"

#include "graphics/render_pipeline_state.h"
#include "graphics/texture.h"
#include "graphics/render_pass.h"

namespace renderer::imgui_backend
{
    struct FramebufferDescriptor
    {
        unsigned long sampleCount;
        graphics::PixelFormat colorPixelFormat;
        graphics::PixelFormat depthPixelFormat;
        graphics::PixelFormat stencilPixelFormat;

        // render pass descriptor should have valid first color attachment, depth attachment and stencil attachment
        explicit FramebufferDescriptor(graphics::RenderPassDescriptor const& renderPassDescriptor)
        {
            assert(!renderPassDescriptor.colorAttachments.empty() && renderPassDescriptor.colorAttachments[0].pTexture);
            assert(renderPassDescriptor.depthAttachment.pTexture);
            assert(renderPassDescriptor.stencilAttachment.pTexture);

            sampleCount = renderPassDescriptor.colorAttachments[0].pTexture->getSampleCount();
            colorPixelFormat = renderPassDescriptor.colorAttachments[0].pTexture->getPixelFormat();
            depthPixelFormat = renderPassDescriptor.depthAttachment.pTexture->getPixelFormat();
            stencilPixelFormat = renderPassDescriptor.stencilAttachment.pTexture->getPixelFormat();
        }
    };

    [[nodiscard]] bool operator==(FramebufferDescriptor const& lhs, FramebufferDescriptor const& rhs)
    {
        return (lhs.sampleCount == rhs.sampleCount &&
                lhs.colorPixelFormat == rhs.colorPixelFormat &&
                lhs.depthPixelFormat == rhs.depthPixelFormat &&
                lhs.stencilPixelFormat == rhs.stencilPixelFormat
        );
    }

    struct BackendData
    {
        graphics::IDevice* pDevice;
        std::unique_ptr<graphics::IDepthStencilState> pDepthStencilState;
        std::unique_ptr<graphics::ITexture> pFontTexture;
        std::unique_ptr<FramebufferDescriptor> pFramebufferDescriptor;
        std::unordered_map<int, std::unique_ptr<graphics::IRenderPipelineState>> renderPipelineStateCache;

        BackendData() {}
    };

    static BackendData* getBackendData()
    {
        if (ImGui::GetCurrentContext())
        {
            return (BackendData*)ImGui::GetIO().BackendRendererUserData;
        }
        else
        {
            return nullptr;
        }
    }

    bool init(graphics::IDevice* pDevice)
    {
        auto* bd = new BackendData();
        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererUserData = (void*)bd;
        io.BackendRendererName = "imgui_impl_shapereality";
        //io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

        bd->pDevice = pDevice;

        return true;
    }

    void shutdown()
    {
        BackendData* bd = getBackendData();
        IM_ASSERT(bd != nullptr && "No renderer backend to shutdown, or already shut down?");
        destroyDeviceObjects();
        delete bd;

        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererName = nullptr;
        io.BackendRendererUserData = nullptr;
//        io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;
    }

    void newFrame(graphics::RenderPassDescriptor const& renderPassDescriptor)
    {
        BackendData* bd = getBackendData();
        IM_ASSERT(bd != nullptr && "No ShapeReality context. Did you call init() ?");

        if (!bd->pDepthStencilState)
        {
            createDeviceObjects(bd->pDevice);
        }
    }

    static void ImGui_ImplShapeReality_SetupRenderState(ImDrawData* drawData,
                                                        graphics::ICommandBuffer* pCommandBuffer,
                                                        graphics::IRenderPipelineState* pRenderPipelineState,
                                                        graphics::IBuffer* pVertexBuffer,
                                                        size_t vertexBufferOffset)
    {
        BackendData* bd = getBackendData();
        pCommandBuffer->setCullMode(graphics::CullMode::None);
        pCommandBuffer->setDepthStencilState(bd->pDepthStencilState.get());

        // Setup viewport, orthographic projection matrix
        // Our visible imgui space lies from draw_data->DisplayPos (top left) to
        // draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
        graphics::Viewport viewport = {
            .originX = 0.0,
            .originY = 0.0,
            .width = drawData->DisplaySize.x * drawData->FramebufferScale.x,
            .height = drawData->DisplaySize.y * drawData->FramebufferScale.y,
            .zNear = 0.0,
            .zFar = 1.0
        };
        pCommandBuffer->setViewport(viewport);

        float L = drawData->DisplayPos.x;
        float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
        float T = drawData->DisplayPos.y;
        float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
        float N = viewport.zNear;
        float F = viewport.zFar;
        const float ortho_projection[4][4] = {
            {2.0f / (R - L),    0.0f,              0.0f,        0.0f},
            {0.0f,              2.0f / (T - B),    0.0f,        0.0f},
            {0.0f,              0.0f,              1 / (F - N), 0.0f},
            {(R + L) / (L - R), (T + B) / (B - T), N / (F - N), 1.0f},
        };
        // todo: [commandEncoder setVertexBytes:&ortho_projection length:sizeof(ortho_projection) atIndex:1];

        pCommandBuffer->setRenderPipelineState(pRenderPipelineState);

        pCommandBuffer->setVertexStageBuffer(pVertexBuffer, /*offset*/ vertexBufferOffset, /*index*/ 0);
    }

    void renderDrawData(ImDrawData* drawData, graphics::ICommandBuffer* pCommandBuffer)
    {
        BackendData* bd = getBackendData();

        // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
        int fb_width = (int)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
        int fb_height = (int)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
        if (fb_width <= 0 || fb_height <= 0 || drawData->CmdListsCount == 0)
        {
            return;
        }
    }

    bool createFontsTexture(graphics::IDevice* pDevice)
    {
        BackendData* bd = getBackendData();
        ImGuiIO& io = ImGui::GetIO();

        // We are retrieving and uploading the font atlas as a 4-channels RGBA texture here.
        // In theory, we could call GetTexDataAsAlpha8() and upload a 1-channel texture to save on memory access bandwidth.
        // However, using a shader designed for 1-channel texture would make it less obvious to use the ImTextureID facility to render users own textures.
        // You can make that change in your implementation.
        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
        graphics::TextureDescriptor textureDescriptor{
            .width = static_cast<unsigned int>(width),
            .height = static_cast<unsigned int>(height),
            .pixelFormat = graphics::PixelFormat::RGBA8Unorm,
            .usage = graphics::TextureUsage_ShaderRead,
            .data = pixels
        };
        bd->pFontTexture = pDevice->createTexture(textureDescriptor);
        io.Fonts->SetTexID(bd->pFontTexture.get()); // ImTextureID == void*

        return true;
    }

    void destroyFontsTexture()
    {
        BackendData* bd = getBackendData();
        ImGuiIO& io = ImGui::GetIO();
        bd->pFontTexture.reset();
        io.Fonts->SetTexID(nullptr);
    }

    bool createDeviceObjects(graphics::IDevice* pDevice)
    {
        BackendData* bd = getBackendData();
        graphics::DepthStencilDescriptor depthStencilDescriptor{
            .depthCompareFunction = graphics::CompareFunction::Always,
            .depthWriteEnabled = false
        };
        bd->pDepthStencilState = pDevice->createDepthStencilState(depthStencilDescriptor);
        createFontsTexture(pDevice);
        return true;
    }

    void destroyDeviceObjects()
    {
        BackendData* bd = getBackendData();
        destroyFontsTexture();
        bd->renderPipelineStateCache.clear(); // automatically releases the render pipeline state objects
    }
}