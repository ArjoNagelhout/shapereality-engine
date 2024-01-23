#ifndef SHAPEREALITY_APPLICATION_H
#define SHAPEREALITY_APPLICATION_H

#include <memory>

namespace graphics
{
    // on macOS the operating system calls the application instead of the other way around, so we'll adopt this
    // approach for our framework as well.
    class IApplicationDelegate
    {
    public:
        virtual void applicationDidFinishLaunching() = 0;

        virtual void applicationWillTerminate() = 0;
    };

    class Application final
    {
    public:
        explicit Application();

        ~Application();

        void run();

        [[nodiscard]] IApplicationDelegate* getDelegate() const;

        void setDelegate(IApplicationDelegate* delegate);

        struct Implementation;

    private:
        std::unique_ptr<Implementation> pImplementation;

        IApplicationDelegate* pDelegate{nullptr};
    };

    // built in cursors
    enum class Cursor
    {
        Arrow,
        IBeam,
        Crosshair,
        ClosedHand,
        OpenHand,
        PointingHand,
        ResizeLeft,
        ResizeRight,
        ResizeLeftOrRight,
        ResizeUp,
        ResizeDown,
        ResizeUpOrDown,
        DisappearingItem, // poof
        IBeamForVerticalLayout,
        OperationNotAllowed,
        DragLink, // alias
        DragCopy,
        ContextMenu
    };

    // sets the cursor, not tied to a window
    void setCursor(Cursor cursor);

    // unhides the cursor
    void showCursor();

    // hides the cursor
    void hideCursor();
}

#endif //SHAPEREALITY_APPLICATION_H