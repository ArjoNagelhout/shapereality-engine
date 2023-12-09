//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_WINDOW_H
#define BORED_ENGINE_MTL_WINDOW_H

#include "../../window.h"

#import <MetalKit/MetalKit.h>

@interface MTKViewDelegate : NSObject<MTKViewDelegate>
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

	private:
		MTKView* _Nonnull pMTKView;
		MTKViewDelegate* _Nullable  pMTKViewDelegate;
	};
}

#endif //BORED_ENGINE_MTL_WINDOW_H
