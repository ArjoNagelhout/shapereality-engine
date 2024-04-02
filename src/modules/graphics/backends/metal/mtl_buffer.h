//
// Created by Arjo Nagelhout on 14/12/2023.
//

#ifndef SHAPEREALITY_MTL_BUFFER_H
#define SHAPEREALITY_MTL_BUFFER_H

#include <graphics/buffer.h>
#include <graphics/device.h>

#import <Metal/Metal.h>

namespace graphics::metal
{
    class MetalBuffer final : public Buffer
    {
    public:
        explicit MetalBuffer(IDevice const* device, BufferDescriptor const& descriptor,
                             void* _Nonnull source, bool take);

        explicit MetalBuffer(IDevice const* device, BufferDescriptor const& descriptor);

        //
        ~MetalBuffer() override;

        // Buffer implementation

        void set(void* _Nonnull source,
                 size_t size,
                 size_t offset,
                 bool synchronize) override;

        [[nodiscard]] void* _Nonnull take() override;

        [[nodiscard]] void* _Nonnull get() override;

        void synchronize(size_t size, size_t offset) override;

        [[nodiscard]] bool requiresSynchronization() const override;

        // Metal specific functions

        // returns the metal buffer object
        [[nodiscard]] id <MTLBuffer> _Nonnull metalBuffer() const;

    private:
        id <MTLBuffer> _Nonnull buffer;
        MTLStorageMode storageMode_;
    };

    [[nodiscard]] MTLIndexType indexType(size_t size);
}

#endif //SHAPEREALITY_MTL_BUFFER_H
