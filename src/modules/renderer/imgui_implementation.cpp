//
// Created by Arjo Nagelhout on 20/01/2024.
//

#include "imgui_implementation.h"

#include "graphics/render_pipeline_state.h"
#include "graphics/texture.h"
#include "graphics/render_pass.h"
#include "graphics/buffer.h"
#include "graphics/shader.h"
#include "graphics/window.h"

#include <chrono>

using namespace graphics;

namespace renderer::imgui_backend
{
    struct FramebufferDescriptor
    {
        std::uint8_t sampleCount; // max sampleCount is 8, so 8 bits is enough
        PixelFormat colorPixelFormat;
        PixelFormat depthPixelFormat;
        PixelFormat stencilPixelFormat;

        explicit FramebufferDescriptor() = default;

        // render pass descriptor should have valid first color attachment, depth attachment and stencil attachment
        explicit FramebufferDescriptor(RenderPassDescriptor const& descriptor)
        {
//            assert(!descriptor.colorAttachments.empty() && descriptor.colorAttachments[0].pTexture);
//            assert(descriptor.depthAttachment.pTexture);
//            assert(descriptor.stencilAttachment.pTexture);

            auto* color = descriptor.colorAttachments[0].pTexture.get();
            auto* depth = descriptor.depthAttachment.pTexture.get();
            auto* stencil = descriptor.stencilAttachment.pTexture.get();

            sampleCount = color ? color->getSampleCount() : 1;
            colorPixelFormat = color ? color->getPixelFormat() : PixelFormat::Undefined;
            depthPixelFormat = depth ? depth->getPixelFormat() : PixelFormat::Undefined;
            stencilPixelFormat = stencil ? stencil->getPixelFormat() : PixelFormat::Undefined;
        }
    };
}

template<>
struct std::hash<renderer::imgui_backend::FramebufferDescriptor>
{
    std::uint32_t operator()(renderer::imgui_backend::FramebufferDescriptor const& d) const
    {
        constexpr int kBitsPerEnum = 8;
        constexpr std::uint32_t kEnumMask = (1u << kBitsPerEnum) - 1;

        // pixel format max value = 138, which fits in 2^8
        auto cf = (static_cast<std::uint32_t>(d.colorPixelFormat) & kEnumMask) << (kBitsPerEnum * 3);
        auto df = (static_cast<std::uint32_t>(d.depthPixelFormat) & kEnumMask) << (kBitsPerEnum * 2);
        auto sf = (static_cast<std::uint32_t>(d.stencilPixelFormat) & kEnumMask) << (kBitsPerEnum * 1);
        auto sc = static_cast<std::uint32_t>(d.sampleCount) & kEnumMask;

        std::uint32_t hash = cf | df | sf | sc;
        return hash;
    }
};

namespace renderer::imgui_backend
{
    using time_type = std::chrono::time_point<std::chrono::system_clock>;
    using duration_type = std::chrono::system_clock::duration;

    time_type getCurrentTime()
    {
        return std::chrono::system_clock::now();
    }

    [[nodiscard]] bool operator==(FramebufferDescriptor const& lhs, FramebufferDescriptor const& rhs)
    {
        return (lhs.sampleCount == rhs.sampleCount &&
                lhs.colorPixelFormat == rhs.colorPixelFormat &&
                lhs.depthPixelFormat == rhs.depthPixelFormat &&
                lhs.stencilPixelFormat == rhs.stencilPixelFormat
        );
    }

    struct Buffer
    {
        std::unique_ptr<IBuffer> pBuffer;
        time_type lastReuseTime;

        explicit Buffer(std::unique_ptr<IBuffer> _pBuffer)
            : pBuffer(std::move(_pBuffer)), lastReuseTime(getCurrentTime()) {}
    };

    struct BackendData
    {
        // io
        graphics::Window* pWindow{nullptr}; // single-window support for now

        // rendering
        IDevice* pDevice{nullptr};
        IShaderLibrary* pShaderLibrary{nullptr};
        std::unique_ptr<IDepthStencilState> pDepthStencilState;
        std::unique_ptr<ITexture> pFontTexture;
        FramebufferDescriptor framebufferDescriptor{}; // current frame buffer descriptor

        // cache with a render pipeline state for each framebuffer descriptor
        std::unordered_map<FramebufferDescriptor, std::unique_ptr<IRenderPipelineState>> renderPipelineStateCache;

        // reusable buffer cache
        std::vector<Buffer> bufferCache;
        std::mutex bufferCacheMutex;
        time_type lastBufferCachePurge{};

        explicit BackendData() : lastBufferCachePurge(getCurrentTime())
        {}

        // returns non-owning pointer
        [[nodiscard]] Buffer dequeueReusableBufferOfLength(size_t length);
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

    void onEvent(InputEvent const& event)
    {

    }

    bool init(IDevice* pDevice, Window* pWindow, IShaderLibrary* pShaderLibrary)
    {
        auto* bd = new BackendData();
        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererUserData = (void*)bd;
        io.BackendRendererName = "imgui_impl_shapereality";
        //io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

        bd->pDevice = pDevice;
        bd->pWindow = pWindow;
        bd->pShaderLibrary = pShaderLibrary;

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

    void newFrame(RenderPassDescriptor const& renderPassDescriptor)
    {
        BackendData* bd = getBackendData();
        IM_ASSERT(bd != nullptr && "No ShapeReality context. Did you call init() ?");
        bd->framebufferDescriptor = FramebufferDescriptor(renderPassDescriptor);

        if (!bd->pDepthStencilState)
        {
            createDeviceObjects(bd->pDevice);
        }

        updateIO();
    }

    void updateIO()
    {
        BackendData* bd = getBackendData();
        ImGuiIO& io = ImGui::GetIO();

        // Setup display size (every frame to accommodate for window resizing)

        math::Rect rect = bd->pWindow->getRect();
        io.DisplaySize = ImVec2(rect.width, rect.height);
        io.DisplayFramebufferScale = ImVec2(1.f, 1.f);
    }

    static void setupRenderState(ImDrawData* drawData,
                                 ICommandBuffer* pCommandBuffer,
                                 IRenderPipelineState* pRenderPipelineState,
                                 IBuffer* pVertexBuffer,
                                 size_t vertexBufferOffset)
    {
        BackendData* bd = getBackendData();
        pCommandBuffer->setCullMode(CullMode::None);
        pCommandBuffer->setDepthStencilState(bd->pDepthStencilState.get());

        // Setup viewport, orthographic projection matrix
        // Our visible imgui space lies from draw_data->DisplayPos (top left) to
        // draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
        Viewport viewport = {
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

    // Bilinear sampling is required by default. Set 'io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines' or 'style.AntiAliasedLinesUseTex = false' to allow point/nearest sampling.
    static std::unique_ptr<IRenderPipelineState>
    createRenderPipelineStateForFramebufferDescriptor(IDevice* pDevice,
                                                      IShaderLibrary* pShaderLibrary,
                                                      FramebufferDescriptor const& framebufferDescriptor)
    {
        ShaderFunctionDescriptor vertexFunctionDescriptor{
            .entryPoint = "imgui_vertex",
            .type = ShaderFunctionType::Vertex
        };
        std::unique_ptr<IShaderFunction> vertexFunction = pShaderLibrary->createShaderFunction(vertexFunctionDescriptor);

        ShaderFunctionDescriptor fragmentFunctionDescriptor{
            .entryPoint = "imgui_fragment",
            .type = ShaderFunctionType::Fragment
        };
        std::unique_ptr<IShaderFunction> fragmentFunction = pShaderLibrary->createShaderFunction(fragmentFunctionDescriptor);

        std::unique_ptr<VertexDescriptor> vertexDescriptor = std::make_unique<VertexDescriptor>();
        vertexDescriptor->attributes = {
            VertexAttributeDescriptor{ // position
                .format = VertexFormat::Float2,
                .offset = offsetof(ImDrawVert, pos),
                .bufferIndex = 0
            },
            VertexAttributeDescriptor{ // texCoords
                .format = VertexFormat::Float2,
                .offset = offsetof(ImDrawVert, uv),
                .bufferIndex = 0
            },
            VertexAttributeDescriptor{ // color
                .format = VertexFormat::UChar4,
                .offset = offsetof(ImDrawVert, col),
                .bufferIndex = 0
            }
        };
        vertexDescriptor->layouts = {
            VertexBufferLayoutDescriptor{
                .stepFunction = VertexStepFunction::PerVertex,
                .stepRate = 1,
                .stride = sizeof(ImDrawVert)
            }
        };
        
        RenderPipelineDescriptor renderPipelineDescriptor{
            .vertexFunction = vertexFunction.get(),
            .fragmentFunction = fragmentFunction.get(),
            .vertexDescriptor = std::move(vertexDescriptor),
            .colorAttachments = {
                RenderPipelineDescriptor::ColorAttachmentDescriptor{
                    .pixelFormat = framebufferDescriptor.colorPixelFormat,
                    .blendingEnabled = true,
                    .alphaBlendOperation = BlendOperation::Add,
                    .rgbBlendOperation = BlendOperation::Add,
                    .destinationAlphaBlendFactor = BlendFactor::OneMinusSourceAlpha,
                    .destinationRGBBlendFactor = BlendFactor::OneMinusSourceAlpha,
                    .sourceAlphaBlendFactor = BlendFactor::One,
                    .sourceRGBBlendFactor = BlendFactor::SourceAlpha,
                }
            },
            .depthAttachmentPixelFormat = framebufferDescriptor.depthPixelFormat,
            .stencilAttachmentPixelFormat = framebufferDescriptor.stencilPixelFormat,
            .rasterSampleCount = framebufferDescriptor.sampleCount,
        };

        return pDevice->createRenderPipelineState(renderPipelineDescriptor);
    }

    void renderDrawData(ImDrawData* drawData, ICommandBuffer* pCommandBuffer)
    {
        BackendData* bd = getBackendData();

        // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
        int fb_width = (int)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
        int fb_height = (int)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
        if (fb_width <= 0 || fb_height <= 0 || drawData->CmdListsCount == 0)
        {
            return;
        }

        // Try to retrieve a render pipeline state that is compatible with the framebuffer config for this frame
        // The hit rate for this cache should be very near 100%.
        if (!bd->renderPipelineStateCache.contains(bd->framebufferDescriptor))
        {
            // No luck; make a new render pipeline state and cache render pipeline state for later reuse
            bd->renderPipelineStateCache[bd->framebufferDescriptor] = createRenderPipelineStateForFramebufferDescriptor(
                bd->pDevice, bd->pShaderLibrary, bd->framebufferDescriptor);
        }
        IRenderPipelineState* pRenderPipelineState =
            bd->renderPipelineStateCache[bd->framebufferDescriptor].get();

        size_t vertexBufferLength = static_cast<size_t>(drawData->TotalVtxCount) * sizeof(ImDrawVert);
        size_t indexBufferLength = static_cast<size_t>(drawData->TotalIdxCount) * sizeof(ImDrawIdx);
        Buffer vertexBuffer = bd->dequeueReusableBufferOfLength(vertexBufferLength);
        Buffer indexBuffer = bd->dequeueReusableBufferOfLength(indexBufferLength);
        indexBuffer.pBuffer->stride = sizeof(ImDrawIdx);

        setupRenderState(drawData, pCommandBuffer, pRenderPipelineState, vertexBuffer.pBuffer.get(), 0);

        // Will project scissor/clipping rectangles into framebuffer space
        ImVec2 clip_off = drawData->DisplayPos;         // (0,0) unless using multi-viewports
        ImVec2 clip_scale = drawData->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

        // Render command lists
        size_t vertexBufferOffset = 0;
        size_t indexBufferOffset = 0;

        for (int n = 0; n < drawData->CmdListsCount; n++)
        {
            ImDrawList const* cmd_list = drawData->CmdLists[n];

            char* vertexBufferContents = static_cast<char*>(vertexBuffer.pBuffer->getContents());
            char* indexBufferContents = static_cast<char*>(indexBuffer.pBuffer->getContents());

            memcpy(vertexBufferContents + vertexBufferOffset, cmd_list->VtxBuffer.Data, static_cast<size_t>(cmd_list->VtxBuffer.Size) * sizeof(ImDrawVert));
            memcpy(indexBufferContents + indexBufferOffset, cmd_list->IdxBuffer.Data, static_cast<size_t>(cmd_list->IdxBuffer.Size) * sizeof(ImDrawIdx));

            for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
            {
                ImDrawCmd const* pcmd = &cmd_list->CmdBuffer[cmd_i];
                if (pcmd->UserCallback)
                {
                    // User callback, registered via ImDrawList::AddCallback()
                    // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                    if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    {
                        setupRenderState(drawData, pCommandBuffer, pRenderPipelineState, vertexBuffer.pBuffer.get(), vertexBufferOffset);
                    }
                    else
                    {
                        pcmd->UserCallback(cmd_list, pcmd);
                    }
                }
                else
                {
                    // Project scissor/clipping rectangles into framebuffer space
                    ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                    ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);

                    // Clamp to viewport as setScissorRect() won't accept values that are off bounds
                    if (clip_min.x < 0.0f) { clip_min.x = 0.0f; }
                    if (clip_min.y < 0.0f) { clip_min.y = 0.0f; }
                    if (clip_max.x > static_cast<float>(fb_width)) { clip_max.x = static_cast<float>(fb_width); }
                    if (clip_max.y > static_cast<float>(fb_height)) { clip_max.y = static_cast<float>(fb_height); }
                    if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    {
                        continue;
                    }

                    if (pcmd->ElemCount == 0) // drawIndexedPrimitives() validation doesn't accept this
                    {
                        continue;
                    }

                    // Apply scissor/clipping rectangle
                    ScissorRect scissorRect = {
                        .x = static_cast<unsigned int>(clip_min.x),
                        .y = static_cast<unsigned int>(clip_min.y),
                        .width = static_cast<unsigned int>(clip_max.x - clip_min.x),
                        .height = static_cast<unsigned int>(clip_max.y - clip_min.y)
                    };
                    pCommandBuffer->setScissorRect(scissorRect);

                    // Bind texture, Draw
                    if (ImTextureID tex_id = pcmd->GetTexID())
                    {
                        auto* texture = static_cast<ITexture*>(tex_id);
                        pCommandBuffer->setFragmentStageTexture(texture, /*atIndex*/ 0);
                    }

                    pCommandBuffer->setVertexStageBufferOffset(vertexBufferOffset + pcmd->VtxOffset * sizeof(ImDrawVert), /*atIndex*/0);

                    pCommandBuffer->drawIndexedPrimitives(PrimitiveType::Triangle,
                        /*indexCount*/ pcmd->ElemCount,
                        /*indexBuffer*/ indexBuffer.pBuffer.get(),
                        /*indexBufferOffset*/ indexBufferOffset + pcmd->IdxOffset * sizeof(ImDrawIdx),
                        /*instanceCount*/ 1,
                        /*baseVertex*/ 0,
                        /*baseInstance*/ 0);
                }
            }

            vertexBufferOffset += (size_t)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert);
            indexBufferOffset += (size_t)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx);
        }
    }

    bool createFontsTexture(IDevice* pDevice)
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
        TextureDescriptor textureDescriptor{
            .width = static_cast<unsigned int>(width),
            .height = static_cast<unsigned int>(height),
            .pixelFormat = PixelFormat::RGBA8Unorm,
            .usage = TextureUsage_ShaderRead,
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

    bool createDeviceObjects(IDevice* pDevice)
    {
        BackendData* bd = getBackendData();
        DepthStencilDescriptor depthStencilDescriptor{
            .depthCompareFunction = CompareFunction::Always,
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

    Buffer BackendData::dequeueReusableBufferOfLength(size_t length)
    {
        time_type now = getCurrentTime();

        std::unique_lock<std::mutex> guard(bufferCacheMutex);

        // Purge old buffers that haven't been useful for a while
        if (now - lastBufferCachePurge > std::chrono::seconds(1))
        {
            std::vector<Buffer> survivors{};
            for (auto& candidate: bufferCache)
            {
                if (candidate.lastReuseTime > lastBufferCachePurge)
                {
                    survivors.emplace_back(std::move(candidate));
                }
            }
            bufferCache = std::move(survivors);
            lastBufferCachePurge = now;
        }

        // See if we have a buffer we can reuse
        auto bestCandidate = bufferCache.end();
        for (auto candidate = bufferCache.begin(); candidate != bufferCache.end(); candidate++)
        {
            if (candidate->pBuffer->getLength() >= length &&
                (bestCandidate == bufferCache.end() || bestCandidate->lastReuseTime > candidate->lastReuseTime))
            {
                bestCandidate = candidate;
            }
        }

        if (bestCandidate != bufferCache.end())
        {
            bestCandidate->lastReuseTime = getCurrentTime();

            // remove from cache
            // we move the buffer first, so that bufferCache.erase doesn't destroy the std::unique_ptr<IBuffer>
            Buffer b = std::move(*bestCandidate);
            bufferCache.erase(bestCandidate);
            return b;
        }

        guard.unlock();

        // No luck; make a new buffer
        BufferDescriptor bufferDescriptor{
            .storageMode = BufferDescriptor::StorageMode::Shared, // this means we don't have to call didModifyRange() after memcpy
            .length = static_cast<unsigned int>(length)
        };
        std::unique_ptr<IBuffer> backing = pDevice->createBuffer(bufferDescriptor);
        return Buffer{std::move(backing)};
    }
}