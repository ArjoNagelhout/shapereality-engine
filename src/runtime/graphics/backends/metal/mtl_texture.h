//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef SHAPEREALITY_MTL_TEXTURE_H
#define SHAPEREALITY_MTL_TEXTURE_H

#include "graphics/texture.h"

#import <Metal/Metal.h>

namespace graphics
{
	class MetalTexture final : public ITexture
	{
	public:
		explicit MetalTexture(TextureDescriptor descriptor);
		~MetalTexture() override;

		explicit MetalTexture(id<MTLDrawable> _Nonnull drawable);

		[[nodiscard]] id<MTLDrawable> _Nonnull getDrawable() const;

	private:
		id<MTLDrawable> _Nullable pDrawable{nullptr};
	};
}

#endif //SHAPEREALITY_MTL_TEXTURE_H