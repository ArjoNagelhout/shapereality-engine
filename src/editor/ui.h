//
// Created by Arjo Nagelhout on 29/01/2024.
//

#ifndef SHAPEREALITY_EDITOR_UI_H
#define SHAPEREALITY_EDITOR_UI_H

#include "graphics/input.h"
#include "graphics/render_pass.h"
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/command_buffer.h"
#include "entity/registry.h"
//#include "reflection/type_info.h"
#include "types.h"

namespace editor
{
    // we implement a simple editor user interface using ImGui, to be replaced later
    // by a custom UI framework
    class UI
    {
    public:
        explicit UI(graphics::IDevice* device, graphics::Window* window, graphics::IShaderLibrary* shaderLibrary);

        ~UI();

        //
        void onEvent(graphics::InputEvent const& event);

        // create new frame and update all UI
        void update(graphics::RenderPassDescriptor const& descriptor);

        //
        void render(graphics::ICommandBuffer* commandBuffer);

        //
        [[nodiscard]] bool getCapturedMouse() const;

        //
        [[nodiscard]] bool getCapturedKeyboard() const;

        // todo: change into referencing scene abstraction
        void setRegistry(entity::Registry* r);

        //
//        void setTypeInfoRegistry(reflection::TypeInfoRegistry* typeInfoRegistry);

    private:
        bool capturedMouse = false;
        bool capturedKeyboard = false;

        entity::Registry* r = nullptr; // todo: change into referencing scene abstraction

//        reflection::TypeInfoRegistry* typeInfoRegistry = nullptr;

        entity::entity_type selectedEntityId = entity::TOMBSTONE;

//        Parent3 testValue;
    };
}

#endif //SHAPEREALITY_EDITOR_UI_H
