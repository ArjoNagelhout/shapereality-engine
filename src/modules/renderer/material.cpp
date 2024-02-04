#include "material.h"

namespace renderer
{
    Material::Material(Shader* shader, graphics::ITexture* texture)
        : shader(shader), texture(texture)
    {

    }

    Material::~Material() = default;

    Shader* Material::getShader() const
    {
        return shader;
    }

    graphics::ITexture* Material::getTexture() const
    {
        return texture;
    }
}