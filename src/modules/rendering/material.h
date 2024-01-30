//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef SHAPEREALITY_MATERIAL_H
#define SHAPEREALITY_MATERIAL_H

#include "graphics/texture.h"
#include "graphics/shader.h"
#include "shader.h"

namespace rendering
{
    class Material
    {
    public:
        explicit Material(Shader* shader, graphics::ITexture* texture);

        ~Material();

        //
        [[nodiscard]] Shader* getShader() const;

        //
        [[nodiscard]] graphics::ITexture* getTexture() const;

    private:
        Shader* shader; // unowned pointer

        // this should be changed to allow for different maps, such as
        // diffuse, normal, specular and ambient occlusion map
        graphics::ITexture* texture; // unowned pointer
    };
}

#endif //SHAPEREALITY_MATERIAL_H
