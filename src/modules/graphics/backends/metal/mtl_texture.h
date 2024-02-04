//
// Created by Arjo Nagelhout on 18/11/2023.
//

#ifndef SHAPEREALITY_MTL_TEXTURE_H
#define SHAPEREALITY_MTL_TEXTURE_H

#include "graphics/texture.h"

#import <Metal/Metal.h>

namespace graphics::metal
{
    class MetalTexture final : public ITexture
    {
    public:
        explicit MetalTexture(id <MTLDevice> _Nonnull device, TextureDescriptor const& descriptor);

        explicit MetalTexture(id <MTLDrawable> _Nonnull drawable);

        // creates a non-owning metal texture, purely for querying texture information
        explicit MetalTexture(id <MTLTexture> _Nonnull texture);

        ~MetalTexture() override;

        [[nodiscard]] id <MTLTexture> _Nonnull get() const;

        [[nodiscard]] id <MTLDrawable> _Nonnull getDrawable() const;

        // ITexture interface

        [[nodiscard]] TextureType getTextureType() const override;

        [[nodiscard]] PixelFormat getPixelFormat() const override;

        [[nodiscard]] unsigned int getWidth() const override;

        [[nodiscard]] unsigned int getHeight() const override;

        [[nodiscard]] unsigned int getDepth() const override;

        [[nodiscard]] unsigned int getMipmapLevelCount() const override;

        [[nodiscard]] unsigned int getArrayLength() const override;

        [[nodiscard]] std::uint8_t getSampleCount() const override;

        [[nodiscard]] bool getIsFramebufferOnly() const override;

        [[nodiscard]] TextureUsage_ getUsage() const override;

    private:
        id <MTLTexture> _Nullable texture{nullptr};
        id <MTLDrawable> _Nullable drawable{nullptr};
    };
}

#endif //SHAPEREALITY_MTL_TEXTURE_H
