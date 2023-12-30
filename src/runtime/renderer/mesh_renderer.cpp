//
// Created by Arjo Nagelhout on 28/12/2023.
//

#include "mesh_renderer.h"

#include "math/matrix.inl"

namespace renderer
{
    MeshRenderer::MeshRenderer(Mesh* pMesh, Material* pMaterial)
        : pMesh(pMesh), pMaterial(pMaterial)
    {

    }

    MeshRenderer::~MeshRenderer() = default;

    void MeshRenderer::updateTransform()
    {
        transform = math::createTranslationRotationScaleMatrix(localPosition, localRotation, localScale);
    }

    math::mat4 MeshRenderer::getTransform()
    {
        if (transformIsDirty)
        {
            updateTransform();
            transformIsDirty = false;
        }

        return transform;
    }

    math::vec3 MeshRenderer::getLocalPosition() const
    {
        return localPosition;
    }

    void MeshRenderer::setLocalPosition(math::vec3 _localPosition)
    {
        localPosition = _localPosition;
    }

    math::Quaternion MeshRenderer::getLocalRotation() const
    {
        return localRotation;
    }

    void MeshRenderer::setLocalRotation(math::Quaternion _localRotation)
    {
        localRotation = _localRotation;
    }

    math::vec3 MeshRenderer::getLocalScale() const
    {
        return localScale;
    }

    void MeshRenderer::setLocalScale(math::vec3 _localScale)
    {
        localScale = _localScale;
    }

    Mesh* MeshRenderer::getMesh() const
    {
        return pMesh;
    }

    Material* MeshRenderer::getMaterial() const
    {
        return pMaterial;
    }
}