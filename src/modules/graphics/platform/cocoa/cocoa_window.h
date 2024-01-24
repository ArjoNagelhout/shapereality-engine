//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef SHAPEREALITY_COCOA_WINDOW_H
#define SHAPEREALITY_COCOA_WINDOW_H

#include "graphics/window.h"

#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

@interface WindowAdapter : NSWindow
@property(unsafe_unretained, nonatomic, nonnull) graphics::Window* pWindow;
@end

@interface ViewAdapter : MTKView
@property(unsafe_unretained, nonatomic, nonnull) graphics::Window* pWindow;
@end

@interface TextInputView : NSView <NSTextInputClient>
@property(unsafe_unretained, nonatomic, nonnull) graphics::Window* pWindow;
- (void)setInputRect:(graphics::Rect const)rect;
@end

namespace graphics
{
    class Window::Implementation
    {
    public:
        WindowAdapter* _Nonnull pWindowAdapter;
        ViewAdapter* _Nonnull pViewAdapter; // there's always only one view inside a window

        // this gets created when text input is needed, with the size of the text input box
        TextInputView* _Nullable pTextInputView;
    };

    [[nodiscard]] std::unique_ptr<Window>
    createWindow(id <MTLDevice> _Nonnull pDevice, WindowDescriptor const& descriptor);
}

#endif //SHAPEREALITY_COCOA_WINDOW_H
