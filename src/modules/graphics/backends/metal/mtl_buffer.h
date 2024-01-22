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
        [[nodiscard]] unsigned int getLength() const override;

        // Metal specific functions

        // returns the buffer
        [[nodiscard]] id <MTLBuffer> _Nonnull get() const;

        // returns the mtl index type based on the provided stride
        [[nodiscard]] MTLIndexType getIndexType() const;

    private:
        id <MTLBuffer> _Nonnull pBuffer;
    };
}

#endif //SHAPEREALITY_MTL_BUFFER_H
