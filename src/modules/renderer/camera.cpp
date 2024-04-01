//
// Created by Arjo Nagelhout on 16/12/2023.
//

#include "camera.h"

#include <iostream>

#include "math/vector.h"
#include "math/vector.inl"
#include "math/matrix.inl"
#include "math/quaternion.inl"
#include "math/utils.h"

namespace renderer
{
    Camera::Camera(graphics::IDevice* device)
    {
        graphics::BufferDescriptor descriptor{
            .usage = graphics::BufferUsage_CPUWrite,
            .size = sizeof(CameraData),
            .stride = sizeof(CameraData)
        };

        buffer = device->createBuffer(descriptor);
    }

    Camera::~Camera() = default;

    CameraProjection Camera::getCameraProjection() const
    {
        return cameraProjection;
    }

    void Camera::setCameraProjection(CameraProjection _cameraProjection)
    {
        cameraProjection = _cameraProjection;
        dirty = true;
    }

    float Camera::getAspectRatio() const
    {
        return aspectRatio;
    }

    void Camera::setAspectRatio(float _aspectRatio)
    {
        aspectRatio = _aspectRatio;
        dirty = true;
    }

    float Camera::getFieldOfView() const
    {
        return fieldOfViewInDegrees;
    }

    void Camera::setFieldOfView(float _fieldOfView)
    {
        fieldOfViewInDegrees = _fieldOfView;
        dirty = true;
    }

    void Camera::setTransform(math::Matrix4 const& _transform)
    {
        transform = _transform;
        dirty = true;
    }

    void Camera::updateBuffer()
    {
        math::Matrix4 view = transform.inverse();

        // perspective projection expects radians!
        math::Matrix4 projection = math::createPerspectiveProjectionMatrix(math::degreesToRadians(fieldOfViewInDegrees),
                                                                           aspectRatio, zNear, zFar);
        math::Matrix4 viewProjection = projection * view;

        // Metal expects matrix to be stored in column major order. So we need to transpose the matrix.
        viewProjection = viewProjection.transpose();

        CameraData cameraData{
            .viewProjection = viewProjection
        };
        buffer->set(&cameraData, sizeof(cameraData), 0, 0, true);
    }

    graphics::Buffer* Camera::getCameraDataBuffer()
    {
        // if dirty, update buffer
        if (dirty)
        {
            updateBuffer();
            dirty = false;
        }

        return buffer.get();
    }
}