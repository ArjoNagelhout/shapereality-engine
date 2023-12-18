//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_WINDOW_H
#define BORED_ENGINE_MTL_WINDOW_H

#include "../../window.h"

#import <MetalKit/MetalKit.h>

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
