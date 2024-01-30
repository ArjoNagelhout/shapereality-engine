#include "graphics/application.h"

#include <iostream>

#import <Cocoa/Cocoa.h>

// this delegate is the Objective-C implementation of NSApplicationDelegate so that
// we can receive events from the NSApplication instance.
@interface NSApplicationDelegate : NSObject <NSApplicationDelegate>
@property(unsafe_unretained, nonatomic) graphics::Application* application;
@end

@implementation NSApplicationDelegate

- (void)applicationWillFinishLaunching:(NSNotification*)notification {

}

- (void)applicationDidFinishLaunching:(NSNotification*)notification {
    _application->getDelegate()->applicationDidFinishLaunching();

    auto* app = (NSApplication*)notification.object;
    [app setActivationPolicy:NSApplicationActivationPolicyRegular];
    [app activateIgnoringOtherApps:YES];
}

- (void)applicationWillTerminate:(NSNotification*)notification {
    _application->getDelegate()->applicationWillTerminate();
}

- (bool)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)sender {
    return YES;
}

@end

namespace graphics
{
    struct Application::Implementation final
    {
        NSApplication* sharedApplication;
        NSApplicationDelegate* delegate;
    };

    Application::Application()
    {
        implementation = std::make_unique<Implementation>();

        // create delegate
        implementation->delegate = [[NSApplicationDelegate alloc] init];
        implementation->delegate.application = this;

        // create application
        implementation->sharedApplication = [NSApplication sharedApplication];
        [implementation->sharedApplication setDelegate:implementation->delegate];
    }

    Application::~Application()
    {
        // destroy delegate and application
        [implementation->delegate release];
        [implementation->sharedApplication release];
    }

    void Application::run()
    {
        [implementation->sharedApplication run];
    }

    void setCursor(Cursor cursor)
    {
        switch (cursor)
        {
            case Cursor::Arrow: [[NSCursor arrowCursor] set]; break;
            case Cursor::IBeam: [[NSCursor IBeamCursor] set]; break;
            case Cursor::Crosshair: [[NSCursor crosshairCursor] set]; break;
            case Cursor::ClosedHand: [[NSCursor closedHandCursor] set]; break;
            case Cursor::OpenHand: [[NSCursor openHandCursor] set]; break;
            case Cursor::PointingHand: [[NSCursor pointingHandCursor] set]; break;
            case Cursor::ResizeLeft: [[NSCursor resizeLeftCursor] set]; break;
            case Cursor::ResizeRight: [[NSCursor resizeRightCursor] set]; break;
            case Cursor::ResizeLeftOrRight: [[NSCursor resizeLeftRightCursor] set]; break;
            case Cursor::ResizeUp: [[NSCursor resizeUpCursor] set]; break;
            case Cursor::ResizeDown: [[NSCursor resizeDownCursor] set]; break;
            case Cursor::ResizeUpOrDown: [[NSCursor resizeUpDownCursor] set]; break;
            case Cursor::DisappearingItem: [[NSCursor disappearingItemCursor] set]; break;
            case Cursor::IBeamForVerticalLayout: [[NSCursor IBeamCursorForVerticalLayout] set]; break;
            case Cursor::OperationNotAllowed: [[NSCursor operationNotAllowedCursor] set]; break;
            case Cursor::DragLink: [[NSCursor dragLinkCursor] set]; break;
            case Cursor::DragCopy: [[NSCursor dragCopyCursor] set]; break;
            case Cursor::ContextMenu: [[NSCursor contextualMenuCursor] set]; break;
        }
    }

    // global counter for how many times we have hidden the cursor, so that when
    // calling unhide cursor, it always works.
    unsigned int hideCount = 0;

    void unhideCursor()
    {
        for (int i = 0; i < hideCount; i++)
        {
            [NSCursor unhide];
        }
        hideCount = 0;
    }

    void hideCursor()
    {
        [NSCursor hide];
        hideCount++;
    }
}