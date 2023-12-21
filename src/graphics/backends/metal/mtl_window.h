//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_WINDOW_H
#define BORED_ENGINE_MTL_WINDOW_H

#include "../../window.h"

#import <MetalKit/MetalKit.h>

/*
 *
 * Inheritance hierarchy for a window:
 *
 * Ideally we have graphics::Window containing an NSWindow* and a view with a metal layer
 *
 * CAMetalLayer
 *
 * The main problem I'm currently running in to is reasoning about whether to put the logic for
 * creating a Metal Window inside backends/metal/mtl_window.h or platform/cocoa/cocoa_window.h
 *
 * The latter approach is taken by SDL, where for each rendering backend they have a different window
 * subclass. (e.g. SDL_cocoametalview : NSView and then on uikit SDL_uikitmetalview : UIView).
 *
 * The reason this is complicated is that we need to not just present things on screen, for which
 * having one subclass MetalWindow : Window would suffice, but we also need to get input from the user.
 *
 * Initially, my idea was to use the pimpl ideom where a Window::Implementation contains the Cocoa or UIKit
 * specific implementation, but we need to override NSView or UIView's methods for getting input.
 *
 * These are for example onMouse(), onKeyDown(), onKeyUp() and onTouchesBegan().
 * Mouse and keyboard events can be intercepted by overriding sendEvent() in a subclass of NSWindow, but
 * for touches this is not possible.
 *
 * Whoops, apparently this is totally possible. sendEvent() on UIWindow.
 *
 * Okay this is not a good approach. Why? Because we need to handle the events and make sure they get used,
 * otherwise each key will cause a beep on macOS.
 */

@interface MetalView : MTKView
@property (unsafe_unretained, nonatomic, nonnull) graphics::Window* pWindow;
@end

namespace graphics
{
	class MetalWindow final : public Window
	{
	public:
		explicit MetalWindow(WindowDescriptor descriptor, id<MTLDevice> _Nonnull pDevice);
		~MetalWindow() override;

		[[nodiscard]] std::unique_ptr<IRenderPass> getRenderPass() const override;

		[[nodiscard]] std::unique_ptr<ITexture> getDrawable() const override;

	private:
		MetalView* _Nonnull pMetalView;
	};
}

#endif //BORED_ENGINE_MTL_WINDOW_H
