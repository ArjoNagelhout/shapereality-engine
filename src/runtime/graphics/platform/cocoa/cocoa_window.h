//
// Created by Arjo Nagelhout on 19/11/2023.
//

#ifndef BORED_ENGINE_COCOA_WINDOW_H
#define BORED_ENGINE_COCOA_WINDOW_H

#include "graphics/window.h"

#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

@interface WindowAdapter : NSWindow
@property (unsafe_unretained, nonatomic, nonnull) graphics::Window* pWindow;
@end

@interface ViewAdapter : MTKView <NSTextInputClient>
@property (unsafe_unretained, nonatomic, nonnull) graphics::Window* pWindow;
@end

namespace graphics
{
	struct Window::Implementation
	{
		WindowAdapter* _Nonnull pWindowAdapter;
		ViewAdapter* _Nonnull pViewAdapter; // there's always only one view inside a window
	};

	[[nodiscard]] std::unique_ptr<Window> createWindow(WindowDescriptor const& descriptor, id<MTLDevice> _Nonnull pDevice);
}

#endif //BORED_ENGINE_COCOA_WINDOW_H
