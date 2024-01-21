#ifndef SHAPEREALITY_WINDOW_H
#define SHAPEREALITY_WINDOW_H

#include <memory>

#include "math/rect.h"
#include "math/vector.h"

#include "input.h"

namespace graphics
{
    enum WindowFlags_ : uint32_t
    {
        WindowFlags_None = 0,
        WindowFlags_Borderless = 1 << 0,
        WindowFlags_Titled = 1 << 1,
        WindowFlags_Closable = 1 << 2,
        WindowFlags_Miniaturizable = 1 << 3,
        WindowFlags_Resizable = 1 << 4,
        WindowFlags_UnifiedTitleAndToolbar = 1 << 5,

        WindowFlags_Default =
        WindowFlags_Titled | WindowFlags_Closable | WindowFlags_Miniaturizable | WindowFlags_Resizable
    };

    class Window;

    class IWindowRenderDelegate
    {
    public:
        virtual void render(Window* window) = 0;
    };

    class IWindowInputDelegate
    {
    public:
        virtual void onEvent(InputEvent const& event, Window* window) = 0;
    };

    struct WindowDescriptor
    {
        int x;
        int y;
        int width;
        int height;
        int flags;
        math::Vector4 clearColor;
    };

    class RenderPassDescriptor;

    class ITexture;

    class Window final
    {
    public:
        explicit Window();

        ~Window();

        // get a render pass descriptor that can be used to draw to this window
        [[nodiscard]] std::unique_ptr<RenderPassDescriptor> getRenderPassDescriptor() const;

        [[nodiscard]] std::unique_ptr<ITexture> getDrawable() const;

        // set the title of this window
        void setTitle(std::string const& title);

        // show this window (on macOS: deminiaturize)
        void show();

        // hide this window (on macOS: minimize)
        void hide();

        // move and scale this window so that it fills the entire screen
        // note: this is not the same as `fullscreen()`
        void maximize();

        // move the window to the front
        void makeKeyAndOrderFront();

        // make this window fullscreen
        void fullscreen();

        //
        void setPosition(int x, int y);

        // set the size of this window
        void setSize(int width, int height);

        // set the minimum horizontal and vertical dimensions of this window when resizing
        void setMinSize(int width, int height);

        // set the maximum horizontal and vertical dimensions of this window when resizing
        void setMaxSize(int width, int height);

        // get the position and size of this window
        [[nodiscard]] math::Rect getRect() const;

        //
        void setRect(math::Rect const& rect); // set both position and size

        // get the render delegate of this window
        [[nodiscard]] IWindowRenderDelegate* getRenderDelegate() const;

        // set the render delegate of this window
        void setRenderDelegate(IWindowRenderDelegate* renderDelegate);

        // get the input delegate of this window
        [[nodiscard]] IWindowInputDelegate* getInputDelegate() const;

        // set the input delegate of this window
        void setInputDelegate(IWindowInputDelegate* inputDelegate);

        // platform specific implementation that can be accessed by a graphics backend subclass of IWindow
        // to automatically create a surface / context that can be drawn to.
        struct Implementation;

        [[nodiscard]] Implementation* getImplementation() const;

    protected:
        std::unique_ptr<Implementation> pImplementation;

        // the delegate is responsible for rendering this window's contents
        IWindowRenderDelegate* pRenderDelegate{nullptr};

        IWindowInputDelegate* pInputDelegate{nullptr};
    };
}

#endif //SHAPEREALITY_WINDOW_H
