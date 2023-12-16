//
// Created by Arjo Nagelhout on 14/12/2023.
//

#ifndef BORED_ENGINE_MTL_BUFFER_H
#define BORED_ENGINE_MTL_BUFFER_H

#include "../../buffer.h"

#import <Metal/Metal.h>

namespace graphics
{
	class MetalBuffer final : public IBuffer
	{
	public:
		explicit MetalBuffer(BufferDescriptor const& descriptor, id<MTLDevice> _Nonnull pDevice);
		~MetalBuffer() override;

		[[nodiscard]] id<MTLBuffer> _Nonnull getBuffer() const;

		// if this buffer is used as an index buffer, this can be called
		[[nodiscard]] MTLIndexType getIndexType() const;

		[[nodiscard]] void* getContents() override;

		void didModifyRange(Range range) override;

	private:
		id<MTLBuffer> _Nonnull pBuffer;

		MTLIndexType indexType;
	};
}

#endif //BORED_ENGINE_MTL_BUFFER_H
