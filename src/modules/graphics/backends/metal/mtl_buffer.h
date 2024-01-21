//
// Created by Arjo Nagelhout on 14/12/2023.
//

#ifndef SHAPEREALITY_MTL_BUFFER_H
#define SHAPEREALITY_MTL_BUFFER_H

#include "graphics/buffer.h"

#import <Metal/Metal.h>

namespace graphics
{
    class MetalBuffer final : public IBuffer
    {
    public:
        explicit MetalBuffer(id <MTLDevice> _Nonnull pDevice, BufferDescriptor const& descriptor);

        //
        ~MetalBuffer() override;

        // IBuffer implementation

        //
        [[nodiscard]] void* _Nullable getContents() override;

        //
        void didModifyRange(Range range) override;

        //
        unsigned int getLength() const override;

        // Metal specific functions

        //
        [[nodiscard]] id <MTLBuffer> _Nonnull getBuffer() const;

        // if this buffer is used as an index buffer, this can be called
        [[nodiscard]] MTLIndexType getIndexType() const;

    private:
        id <MTLBuffer> _Nonnull pBuffer;

        MTLIndexType indexType;
    };
}

#endif //SHAPEREALITY_MTL_BUFFER_H
