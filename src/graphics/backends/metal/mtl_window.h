//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef BORED_ENGINE_MTL_WINDOW_H
#define BORED_ENGINE_MTL_WINDOW_H

#include "../../window.h"

#import <MetalKit/MetalKit.h>

@interface MTKViewDelegate : NSObject<MTKViewDelegate>
@property (unsafe_unretained, nonatomic, nonnull) graphics::IWindow* pWindow;
@end

namespace graphics
{
	class MetalWindow final : public IWindow
	{
	public:
		explicit MetalWindow(WindowDescription description, id<MTLDevice> _Nonnull pDevice);
		~MetalWindow() override;

	private:
		MTKView* _Nonnull pMTKView;
		MTKViewDelegate* _Nullable  pMTKViewDelegate;
	};
}

#endif //BORED_ENGINE_MTL_WINDOW_H
