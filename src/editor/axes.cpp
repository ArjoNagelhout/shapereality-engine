//
// Created by Arjo Nagelhout on 26/04/2024.
//

#include "axes.h"

#include <math/vector.inl>

namespace editor
{
    asset::Asset createAxesMesh(asset::AssetDatabase& assets, asset::AssetId id)
    {
        // Manual creation of a triangle
        renderer::MeshDescriptor descriptor{
            .primitiveType = graphics::PrimitiveType::Triangle,
            .attributes{
                renderer::VertexAttributeDescriptor{
                    .index = 0,
                    .type = renderer::VertexAttribute_Position,
                    .elementType = renderer::ElementType::Vector3,
                    .componentType = renderer::ComponentType::Float
                },
                renderer::VertexAttributeDescriptor{
                    .index = 0,
                    .type = renderer::VertexAttribute_Color,
                    .elementType = renderer::ElementType::Vector3,
                    .componentType = renderer::ComponentType::Float
                }
            },
            .vertexCount = 6 * 3,
            .hasIndexBuffer = true,
            .indexCount = 6 * 3,
            .indexType = renderer::ComponentType::UnsignedInt,
            .writable = true,
        };
        asset::Asset result = asset::makeAsset<renderer::Mesh>(std::move(id), assets.context().device, descriptor);
        {
            auto& mesh = result->get<renderer::Mesh>();
            std::vector<math::Vector3> positions;
            std::vector<math::Vector3> colors;
            std::vector<unsigned int> indices;
            std::vector<unsigned int> indicesTemplate{
                0, 1, 2, 1, 3, 2
            };

            float w = 1.0f; // width
            float l = 10.0f; // length

            // positions
            positions = {
                // x
                math::Vector3{l, -w, 0},
                math::Vector3{l, +w, 0},
                math::Vector3{0, -w, 0},
                math::Vector3{0, +w, 0},

                // y
                math::Vector3{-w, l, 0},
                math::Vector3{+w, l, 0},
                math::Vector3{-w, 0, 0},
                math::Vector3{+w, 0, 0},

                // z
                math::Vector3{-w, 0, l},
                math::Vector3{+w, 0, l},
                math::Vector3{-w, 0, 0},
                math::Vector3{+w, 0, 0},
            };

            for (int i = 0; i <= 2; i++)
            {
                // colors
                math::Vector3 c;
                c[i] = 1.0f;
                for (int j = 0; j < 4; j++)
                {
                    colors.emplace_back(c);
                }

                // indices
                for (auto& index: indicesTemplate)
                {
                    indices.emplace_back(index + 4 * i);
                }
            }

            mesh.setAttributeData(renderer::VertexAttribute_Position, positions.data(), 0);
            mesh.setAttributeData(renderer::VertexAttribute_Color, colors.data(), 0);
            mesh.setIndexData(indices.data());
        }
        return result;
    }
}