//
// Created by Arjo Nagelhout on 20/01/2024.
//

#include "imgui_backend.h"

#include <graphics/render_pipeline_state.h>
#include <graphics/texture.h>
#include <graphics/render_pass.h>
#include <graphics/buffer.h>
#include <graphics/shader.h>
#include <graphics/window.h>
#include <graphics/application.h>

#include <iostream>
#include <chrono>
#include <unordered_map>

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
            auto* color = descriptor.colorAttachments[0].texture.get();
            auto* depth = descriptor.depthAttachment.texture.get();
            auto* stencil = descriptor.stencilAttachment.texture.get();

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
    using duration_type = time_type::duration;

    constexpr float kScrollMultiplier = 0.02f;

    time_type getCurrentTime()
    {
        return std::chrono::system_clock::now();
    }

    static ImGuiKey convert(graphics::KeyCode value);

    static int convert(graphics::MouseButton value);

    [[nodiscard]] bool operator==(FramebufferDescriptor const& lhs, FramebufferDescriptor const& rhs)
    {
        return (lhs.sampleCount == rhs.sampleCount &&
                lhs.colorPixelFormat == rhs.colorPixelFormat &&
                lhs.depthPixelFormat == rhs.depthPixelFormat &&
                lhs.stencilPixelFormat == rhs.stencilPixelFormat
        );
    }

    struct ReusableBuffer
    {
        std::unique_ptr<graphics::Buffer> buffer;
        time_type lastReuseTime;

        explicit ReusableBuffer(std::unique_ptr<graphics::Buffer> buffer)
            : buffer(std::move(buffer)), lastReuseTime(getCurrentTime()) {}
    };

    struct BackendData
    {
        // io
        graphics::Window* window{nullptr}; // only single-window support for now
        Cursor lastCursor{Cursor::Arrow};
        time_type lastFrameTime{getCurrentTime()};
        std::string clipboardTextData{};

        // rendering
        IDevice* device{nullptr};
        IShaderLibrary* shaderLibrary{nullptr};
        std::unique_ptr<IDepthStencilState> depthStencilState;
        std::unique_ptr<ITexture> fontTexture;
        FramebufferDescriptor framebufferDescriptor{}; // current frame buffer descriptor

        // cache with a render pipeline state for each framebuffer descriptor
        std::unordered_map<FramebufferDescriptor, std::unique_ptr<IRenderPipelineState>> renderPipelineStateCache;

        // reusable buffer cache
        std::vector<ReusableBuffer> bufferCache;
        std::mutex bufferCacheMutex;
        time_type lastBufferCachePurge{};

        explicit BackendData() : lastBufferCachePurge(getCurrentTime()) {}

        // returns non-owning pointer
        [[nodiscard]] ReusableBuffer dequeueReusableBufferOfLength(size_t length);
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

    Cursor convert(ImGuiMouseCursor_ value)
    {
        switch (value)
        {
            case ImGuiMouseCursor_Arrow: return Cursor::Arrow;
            case ImGuiMouseCursor_TextInput: return Cursor::IBeam;
            case ImGuiMouseCursor_ResizeNS: return Cursor::ResizeUpOrDown;
            case ImGuiMouseCursor_ResizeEW: return Cursor::ResizeLeftOrRight;
            case ImGuiMouseCursor_Hand: return Cursor::PointingHand;
            case ImGuiMouseCursor_NotAllowed: return Cursor::OperationNotAllowed;
                // unhandled cases: (we should use custom cursors for this)
            case ImGuiMouseCursor_None:
            case ImGuiMouseCursor_ResizeAll:
            case ImGuiMouseCursor_ResizeNESW:
            case ImGuiMouseCursor_ResizeNWSE:
            case ImGuiMouseCursor_COUNT: return Cursor::Arrow;
        }
    }

    static void setClipboardText(void*, const char* text)
    {
        // todo: set clipboard text
        std::cout << "set clipboard text" << std::endl;
    }

    static char const* getClipboardText(void*)
    {
        BackendData* bd = getBackendData();
        std::cout << "get clipboard text" << std::endl;
        // todo: get clipboard text
        return bd->clipboardTextData.c_str();
    }

    static void setPlatformImeData(ImGuiViewport*, ImGuiPlatformImeData* data)
    {
        std::cout << "set platform IME data:"
                  << " WantVisible: " << data->WantVisible << "\n"
                  << " InputPos: (" << data->InputPos.x << ", " << data->InputPos.y << ")\n"
                  << " InputLineHeight: " << data->InputLineHeight
                  << std::endl;

        BackendData* bd = getBackendData();

        if (data->WantVisible)
        {
            Rect r{
                .x = data->InputPos.x,
                .y = data->InputPos.y,
                .width = 1,
                .height = data->InputLineHeight
            };
            bd->window->enableTextInput();
            bd->window->setTextInputRect(r);
        }
        else
        {
            bd->window->disableTextInput();
        }
    }

    static void updateMouseCursor()
    {
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        {
            return;
        }
        BackendData* bd = getBackendData();
        auto imgui_cursor = static_cast<ImGuiMouseCursor_>(ImGui::GetMouseCursor());
        if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
        {
            // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
            hideCursor();
        }
        else
        {
            // Show OS mouse cursor
            Cursor expected_cursor = convert(imgui_cursor);

            if (bd->lastCursor != expected_cursor)
            {
                setCursor(expected_cursor); // SDL function doesn't have an early out (see #6113)
                bd->lastCursor = expected_cursor;
            }
            unhideCursor();
        }
    }

    void onEvent(InputEvent const& event)
    {
        ImGuiIO& io = ImGui::GetIO();
        switch (event.getType())
        {
            case InputEventType::None: break;
            case InputEventType::Mouse:
            {
                MouseEvent const mouse = event.getMouse();
                int button = convert(mouse.mouseButton);
                float x = mouse.x;
                float y = io.DisplaySize.y - mouse.y; // y is flipped
                switch (mouse.type)
                {
                    case MouseEventType::Up:
                    case MouseEventType::Down:
                    {
                        if (button == -1)
                        {
                            return;
                        }
                        io.AddMouseButtonEvent(button, mouse.type == MouseEventType::Down);
                        return;
                    }
                    case MouseEventType::Moved:
                    {
                        io.AddMousePosEvent(x, y);
                        return;
                    }
                        // todo:
                    case MouseEventType::Entered:
                    case MouseEventType::Exited:return;
                    case MouseEventType::Dragged:
                    {
                        io.AddMousePosEvent(x, y);
                        return;
                    }
                }
            }
            case InputEventType::Scroll:
            {
                ScrollEvent const scroll = event.getScroll();
                io.AddMouseWheelEvent(scroll.x * kScrollMultiplier, scroll.y * kScrollMultiplier);
                return;
            }
            case InputEventType::TextInput:
            {
                TextInputEvent const textInput = event.getTextInput();
                io.AddInputCharactersUTF8(textInput.text.c_str());
                return;
            }
            case InputEventType::TextEditing:
            {
                return;
            }
            case InputEventType::Keyboard:
            {
                KeyboardEvent const keyboard = event.getKeyboard();
                ImGuiKey key = convert(keyboard.keyCode);
                switch (keyboard.type)
                {
                    case KeyboardEventType::Up:
                    {
                        io.AddKeyEvent(key, false);
                        return;
                    }
                    case KeyboardEventType::Down:
                    {
                        io.AddKeyEvent(key, true);
                        return;
                    }
                    case KeyboardEventType::Repeat:break;
                    case KeyboardEventType::ModifiersChanged:
                    {
                        auto modifiers = keyboard.modifiers;
                        io.AddKeyEvent(ImGuiMod_Ctrl, ((modifiers) & KeyboardModifier_Control) != 0);
                        io.AddKeyEvent(ImGuiMod_Shift, ((modifiers) & KeyboardModifier_Shift) != 0);
                        io.AddKeyEvent(ImGuiMod_Alt, ((modifiers) & KeyboardModifier_Option) != 0);
                        io.AddKeyEvent(ImGuiMod_Super, ((modifiers) & KeyboardModifier_Meta) != 0);
                        return;
                    }
                }
            }
        }
    }

    static void initIO(BackendData* bd, ImGuiIO& io)
    {
        io.SetPlatformImeDataFn = setPlatformImeData;
        io.SetClipboardTextFn = setClipboardText;
        io.GetClipboardTextFn = getClipboardText;
    }

    bool init(IDevice* device, Window* window, IShaderLibrary* shaderLibrary)
    {
        auto* bd = new BackendData();
        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererUserData = (void*)bd;
        io.BackendRendererName = "imgui_impl_shapereality";
        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

        bd->device = device;
        bd->window = window;
        bd->shaderLibrary = shaderLibrary;

        initIO(bd, io);

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
        io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;
    }

    void newFrame(RenderPassDescriptor const& renderPassDescriptor)
    {
        BackendData* bd = getBackendData();
        IM_ASSERT(bd != nullptr && "No ShapeReality context. Did you call init() ?");
        bd->framebufferDescriptor = FramebufferDescriptor(renderPassDescriptor);

        if (!bd->depthStencilState)
        {
            createDeviceObjects(bd->device);
        }

        updateIO();
    }

    void updateIO()
    {
        BackendData* bd = getBackendData();
        ImGuiIO& io = ImGui::GetIO();

        // Setup display size (every frame to accommodate for window resizing)

        Size size = bd->window->getContentViewSize();
        io.DisplaySize = ImVec2(size.width, size.height);
        float scaleFactor = bd->window->getScaleFactor();
        io.DisplayFramebufferScale = ImVec2(scaleFactor, scaleFactor);

        time_type now = getCurrentTime();
        duration_type delta = now - bd->lastFrameTime;
        io.DeltaTime = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count()) / 1e9f;
        bd->lastFrameTime = now;

        updateMouseCursor();
    }

    static void setupRenderState(ImDrawData* drawData,
                                 ICommandBuffer* commandBuffer,
                                 IRenderPipelineState* renderPipelineState,
                                 graphics::Buffer* vertexBuffer,
                                 size_t vertexBufferOffset)
    {
        BackendData* bd = getBackendData();
        commandBuffer->setCullMode(CullMode::None);
        commandBuffer->setDepthStencilState(bd->depthStencilState.get());

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
        commandBuffer->setViewport(viewport);

        float L = drawData->DisplayPos.x;
        float R = drawData->DisplayPos.x + drawData->DisplaySize.x;
        float T = drawData->DisplayPos.y;
        float B = drawData->DisplayPos.y + drawData->DisplaySize.y;
        float N = viewport.zNear;
        float F = viewport.zFar;
        float const ortho_projection[4][4] = {
            {2.0f / (R - L), 0.0f, 0.0f, 0.0f},
            {0.0f, 2.0f / (T - B), 0.0f, 0.0f},
            {0.0f, 0.0f, 1 / (F - N), 0.0f},
            {(R + L) / (L - R), (T + B) / (B - T), N / (F - N), 1.0f},
        };

        commandBuffer->setVertexStageBytes(static_cast<void const*>(&ortho_projection), /*length*/
                                           sizeof(ortho_projection), /*atIndex*/1);
        commandBuffer->setRenderPipelineState(renderPipelineState);
        commandBuffer->setVertexStageBuffer(vertexBuffer, /*offset*/ vertexBufferOffset, /*index*/ 0);
    }

    // Bilinear sampling is required by default. Set 'io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines' or 'style.AntiAliasedLinesUseTex = false' to allow point/nearest sampling.
    static std::unique_ptr<IRenderPipelineState>
    createRenderPipelineStateForFramebufferDescriptor(IDevice* device,
                                                      IShaderLibrary* shaderLibrary,
                                                      FramebufferDescriptor const& framebufferDescriptor)
    {
        ShaderFunctionDescriptor vertexFunctionDescriptor{
            .entryPoint = "imgui_vertex",
            .type = ShaderFunctionType::Vertex
        };
        std::unique_ptr<IShaderFunction> vertexFunction = shaderLibrary->createShaderFunction(
            vertexFunctionDescriptor);

        ShaderFunctionDescriptor fragmentFunctionDescriptor{
            .entryPoint = "imgui_fragment",
            .type = ShaderFunctionType::Fragment
        };
        std::unique_ptr<IShaderFunction> fragmentFunction = shaderLibrary->createShaderFunction(
            fragmentFunctionDescriptor);

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

        return device->createRenderPipelineState(renderPipelineDescriptor);
    }

    void renderDrawData(ImDrawData* drawData, ICommandBuffer* commandBuffer)
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
                bd->device, bd->shaderLibrary, bd->framebufferDescriptor);
        }
        IRenderPipelineState* pRenderPipelineState =
            bd->renderPipelineStateCache[bd->framebufferDescriptor].get();

        size_t vertexBufferLength = static_cast<size_t>(drawData->TotalVtxCount) * sizeof(ImDrawVert);
        size_t indexBufferLength = static_cast<size_t>(drawData->TotalIdxCount) * sizeof(ImDrawIdx);
        ReusableBuffer vertexBuffer = bd->dequeueReusableBufferOfLength(vertexBufferLength);
        ReusableBuffer indexBuffer = bd->dequeueReusableBufferOfLength(indexBufferLength);
        indexBuffer.buffer->stride() = sizeof(ImDrawIdx);

        setupRenderState(drawData, commandBuffer, pRenderPipelineState, vertexBuffer.buffer.get(), 0);

        // Will project scissor/clipping rectangles into framebuffer space
        ImVec2 clip_off = drawData->DisplayPos;         // (0,0) unless using multi-viewports
        ImVec2 clip_scale = drawData->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

        // Render command lists
        size_t vertexBufferOffset = 0;
        size_t indexBufferOffset = 0;

        for (int n = 0; n < drawData->CmdListsCount; n++)
        {
            ImDrawList const* cmd_list = drawData->CmdLists[n];

            vertexBuffer.buffer->set(
                /*source*/ cmd_list->VtxBuffer.Data,
                /*size*/ static_cast<size_t>(cmd_list->VtxBuffer.Size) * sizeof(ImDrawVert),
                /*targetOffset*/ vertexBufferOffset,
                /*synchronize*/ true);
            indexBuffer.buffer->set(
                /*source*/ cmd_list->IdxBuffer.Data,
                /*size*/ static_cast<size_t>(cmd_list->IdxBuffer.Size) * sizeof(ImDrawIdx),
                /*targetOffset*/ indexBufferOffset,
                /*synchronize*/ true);

            for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
            {
                ImDrawCmd const* pcmd = &cmd_list->CmdBuffer[cmd_i];
                if (pcmd->UserCallback)
                {
                    // User callback, registered via ImDrawList::AddCallback()
                    // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
                    if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    {
                        setupRenderState(drawData, commandBuffer, pRenderPipelineState, vertexBuffer.buffer.get(),
                                         vertexBufferOffset);
                    }
                    else
                    {
                        pcmd->UserCallback(cmd_list, pcmd);
                    }
                }
                else
                {
                    // Project scissor/clipping rectangles into framebuffer space
                    ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x,
                                    (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                    ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x,
                                    (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);

                    // Clamp to viewport as setScissorRect() won't accept values that are off bounds
                    if (clip_min.x < 0.0f)
                    { clip_min.x = 0.0f; }
                    if (clip_min.y < 0.0f)
                    { clip_min.y = 0.0f; }
                    if (clip_max.x > static_cast<float>(fb_width))
                    { clip_max.x = static_cast<float>(fb_width); }
                    if (clip_max.y > static_cast<float>(fb_height))
                    { clip_max.y = static_cast<float>(fb_height); }
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
                    commandBuffer->setScissorRect(scissorRect);

                    // Bind texture, Draw
                    if (ImTextureID tex_id = pcmd->GetTexID())
                    {
                        auto* texture = static_cast<ITexture*>(tex_id);
                        commandBuffer->setFragmentStageTexture(texture, /*atIndex*/ 0);
                    }

                    commandBuffer->setVertexStageBufferOffset(
                        vertexBufferOffset + pcmd->VtxOffset * sizeof(ImDrawVert), /*atIndex*/0);

                    commandBuffer->drawIndexedPrimitives(PrimitiveType::Triangle,
                        /*indexCount*/ pcmd->ElemCount,
                        /*indexBuffer*/ indexBuffer.buffer.get(),
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

    bool createFontsTexture(IDevice* device)
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
        bd->fontTexture = device->createTexture(textureDescriptor);
        io.Fonts->SetTexID(bd->fontTexture.get()); // ImTextureID == void*

        return true;
    }

    void destroyFontsTexture()
    {
        BackendData* bd = getBackendData();
        ImGuiIO& io = ImGui::GetIO();
        bd->fontTexture.reset();
        io.Fonts->SetTexID(nullptr);
    }

    bool createDeviceObjects(IDevice* device)
    {
        BackendData* bd = getBackendData();
        DepthStencilDescriptor depthStencilDescriptor{
            .depthCompareFunction = CompareFunction::Always,
            .depthWriteEnabled = false
        };
        bd->depthStencilState = device->createDepthStencilState(depthStencilDescriptor);
        createFontsTexture(device);
        return true;
    }

    void destroyDeviceObjects()
    {
        BackendData* bd = getBackendData();
        destroyFontsTexture();
        bd->renderPipelineStateCache.clear(); // automatically releases the render pipeline state objects
    }

    ReusableBuffer BackendData::dequeueReusableBufferOfLength(size_t length)
    {
        time_type now = getCurrentTime();

        std::unique_lock<std::mutex> guard(bufferCacheMutex);

        // Purge old buffers that haven't been useful for a while
        if (now - lastBufferCachePurge > std::chrono::seconds(1))
        {
            std::vector<ReusableBuffer> survivors{};
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
            if (candidate->buffer->descriptor().size >= length &&
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
            ReusableBuffer b = std::move(*bestCandidate);
            bufferCache.erase(bestCandidate);
            return b;
        }

        guard.unlock();

        // No luck; make a new buffer
        BufferDescriptor bufferDescriptor{
            .usage = static_cast<BufferUsage_>(BufferUsage_CPUWrite |
                                               BufferUsage_CPURead | BufferUsage_GPURead | BufferUsage_GPUWrite), // this means we don't have to call didModifyRange() after memcpy
            .size = length
        };
        std::unique_ptr<graphics::Buffer> backing = device->createBuffer(bufferDescriptor);
        return ReusableBuffer{std::move(backing)};
    }

    static ImGuiKey convert(graphics::KeyCode value)
    {
        switch (value)
        {
            case KeyCode::None: return ImGuiKey_None;
            case KeyCode::Space: return ImGuiKey_Space;
            case KeyCode::Enter: return ImGuiKey_Enter;
            case KeyCode::Tab: return ImGuiKey_Tab;
            case KeyCode::Backtick: return ImGuiKey_GraveAccent;
            case KeyCode::Quote: return ImGuiKey_Apostrophe;
            case KeyCode::Semicolon: return ImGuiKey_Semicolon;
            case KeyCode::Comma: return ImGuiKey_Comma;
            case KeyCode::Period: return ImGuiKey_Period;
            case KeyCode::Slash: return ImGuiKey_Slash;
            case KeyCode::Backslash: return ImGuiKey_Backslash;
            case KeyCode::LeftBracket: return ImGuiKey_LeftBracket;
            case KeyCode::RightBracket: return ImGuiKey_RightBracket;
            case KeyCode::Minus: return ImGuiKey_Minus;
            case KeyCode::Equals: return ImGuiKey_Equal;
            case KeyCode::A: return ImGuiKey_A;
            case KeyCode::B: return ImGuiKey_B;
            case KeyCode::C: return ImGuiKey_C;
            case KeyCode::D: return ImGuiKey_D;
            case KeyCode::E: return ImGuiKey_E;
            case KeyCode::F: return ImGuiKey_F;
            case KeyCode::G: return ImGuiKey_G;
            case KeyCode::H: return ImGuiKey_H;
            case KeyCode::I: return ImGuiKey_I;
            case KeyCode::J: return ImGuiKey_J;
            case KeyCode::K: return ImGuiKey_K;
            case KeyCode::L: return ImGuiKey_L;
            case KeyCode::M: return ImGuiKey_M;
            case KeyCode::N: return ImGuiKey_N;
            case KeyCode::O: return ImGuiKey_O;
            case KeyCode::P: return ImGuiKey_P;
            case KeyCode::Q: return ImGuiKey_Q;
            case KeyCode::R: return ImGuiKey_R;
            case KeyCode::S: return ImGuiKey_S;
            case KeyCode::T: return ImGuiKey_T;
            case KeyCode::U: return ImGuiKey_U;
            case KeyCode::V: return ImGuiKey_V;
            case KeyCode::W: return ImGuiKey_W;
            case KeyCode::X: return ImGuiKey_X;
            case KeyCode::Y: return ImGuiKey_Y;
            case KeyCode::Z: return ImGuiKey_Z;
            case KeyCode::Digit1: return ImGuiKey_1;
            case KeyCode::Digit2: return ImGuiKey_2;
            case KeyCode::Digit3: return ImGuiKey_3;
            case KeyCode::Digit4: return ImGuiKey_4;
            case KeyCode::Digit5: return ImGuiKey_5;
            case KeyCode::Digit6: return ImGuiKey_6;
            case KeyCode::Digit7: return ImGuiKey_7;
            case KeyCode::Digit8: return ImGuiKey_8;
            case KeyCode::Digit9: return ImGuiKey_9;
            case KeyCode::Digit0: return ImGuiKey_0;
            case KeyCode::LeftShift: return ImGuiKey_LeftShift;
            case KeyCode::LeftControl: return ImGuiKey_LeftCtrl;
            case KeyCode::LeftOption: return ImGuiKey_LeftAlt;
            case KeyCode::LeftMeta: return ImGuiKey_LeftSuper;
            case KeyCode::RightShift: return ImGuiKey_RightShift;
            case KeyCode::RightControl: return ImGuiKey_RightCtrl;
            case KeyCode::RightOption: return ImGuiKey_RightAlt;
            case KeyCode::RightMeta: return ImGuiKey_RightSuper;
            case KeyCode::ContextMenu: return ImGuiKey_Menu;
            case KeyCode::Escape: return ImGuiKey_Escape;
            case KeyCode::LeftArrow: return ImGuiKey_LeftArrow;
            case KeyCode::RightArrow: return ImGuiKey_RightArrow;
            case KeyCode::UpArrow: return ImGuiKey_UpArrow;
            case KeyCode::DownArrow: return ImGuiKey_DownArrow;
            case KeyCode::Backspace: return ImGuiKey_Backspace;
            case KeyCode::PageDown: return ImGuiKey_PageDown;
            case KeyCode::PageUp: return ImGuiKey_PageUp;
            case KeyCode::Home: return ImGuiKey_Home;
            case KeyCode::End: return ImGuiKey_End;
            case KeyCode::Insert: return ImGuiKey_Insert;
            case KeyCode::Delete: return ImGuiKey_Delete;
            case KeyCode::CapsLock: return ImGuiKey_CapsLock;
            case KeyCode::NumLock: return ImGuiKey_NumLock;
            case KeyCode::PrintScreen: return ImGuiKey_PrintScreen;
            case KeyCode::ScrollLock: return ImGuiKey_ScrollLock;
            case KeyCode::Pause: return ImGuiKey_Pause;
            case KeyCode::NumpadEnter: return ImGuiKey_KeypadEnter;
            case KeyCode::NumpadDivide: return ImGuiKey_KeypadDivide;
            case KeyCode::NumpadMultiply: return ImGuiKey_KeypadMultiply;
            case KeyCode::NumpadPlus: return ImGuiKey_KeypadAdd;
            case KeyCode::NumpadMinus: return ImGuiKey_KeypadSubtract;
            case KeyCode::NumpadPeriod: return ImGuiKey_KeypadDecimal;
            case KeyCode::NumpadEquals: return ImGuiKey_KeypadEqual;
            case KeyCode::Numpad0: return ImGuiKey_Keypad0;
            case KeyCode::Numpad1: return ImGuiKey_Keypad1;
            case KeyCode::Numpad2: return ImGuiKey_Keypad2;
            case KeyCode::Numpad3: return ImGuiKey_Keypad3;
            case KeyCode::Numpad4: return ImGuiKey_Keypad4;
            case KeyCode::Numpad5: return ImGuiKey_Keypad5;
            case KeyCode::Numpad6: return ImGuiKey_Keypad6;
            case KeyCode::Numpad7: return ImGuiKey_Keypad7;
            case KeyCode::Numpad8: return ImGuiKey_Keypad8;
            case KeyCode::Numpad9: return ImGuiKey_Keypad9;
            case KeyCode::F1: return ImGuiKey_F1;
            case KeyCode::F2: return ImGuiKey_F2;
            case KeyCode::F3: return ImGuiKey_F3;
            case KeyCode::F4: return ImGuiKey_F4;
            case KeyCode::F5: return ImGuiKey_F5;
            case KeyCode::F6: return ImGuiKey_F6;
            case KeyCode::F7: return ImGuiKey_F7;
            case KeyCode::F8: return ImGuiKey_F8;
            case KeyCode::F9: return ImGuiKey_F9;
            case KeyCode::F10: return ImGuiKey_F10;
            case KeyCode::F11: return ImGuiKey_F11;
            case KeyCode::F12: return ImGuiKey_F12;
            case KeyCode::F13: return ImGuiKey_F13;
            case KeyCode::F14: return ImGuiKey_F14;
            case KeyCode::F15: return ImGuiKey_F15;
            case KeyCode::F16: return ImGuiKey_F16;
            case KeyCode::F17: return ImGuiKey_F17;
            case KeyCode::F18: return ImGuiKey_F18;
            case KeyCode::F19: return ImGuiKey_F19;
            case KeyCode::F20: return ImGuiKey_F20;
            case KeyCode::OEM1:
            case KeyCode::OEM2:
            case KeyCode::OEM3:
            case KeyCode::OEM4:
            case KeyCode::OEM5: return ImGuiKey_None;
        }
    }

    static int convert(graphics::MouseButton value)
    {
        switch (value)
        {
            case MouseButton::None: return -1;
            case MouseButton::Left: return 0;
            case MouseButton::Right: return 1;
            case MouseButton::Middle: return 2;
        }
    }
}