//
// Created by Arjo Nagelhout on 16/12/2023.
//

#include "camera.h"

#include <iostream>

#include "math/vector.h"
#include "math/vector.inl"
#include "math/matrix.inl"
#include "math/quaternion.inl"
#include "math/utility.h"

namespace renderer
{
    Camera::Camera(graphics::IDevice* device, CameraParameters parameters)
        : parameters_(parameters)
    {
        graphics::BufferDescriptor descriptor{
            .usage = graphics::BufferUsage_CPUWrite,
            .size = sizeof(CameraData),
            .stride = sizeof(CameraData)
        };

        buffer = device->createBuffer(descriptor);
    }

    Camera::~Camera() = default;

    graphics::Buffer* Camera::getCameraDataBuffer()
    {
        updateBuffer();
        return buffer.get();
    }

    CameraParameters& Camera::parameters()
    {
        return parameters_;
    }

    math::Vector3f& Camera::position()
    {
        return position_;
    }

    math::Quaternionf& Camera::rotation()
    {
        return rotation_;
    }

    void Camera::updateBuffer()
    {
        math::Matrix4 t = math::createTranslationMatrix(-position_);
        math::Matrix4 r = math::createRotationMatrix(rotation_);

        math::Matrix4 view = r * t;

        // perspective projection expects radians!
        math::Matrix4 projection = math::createPerspectiveProjectionMatrix(
            math::degreesToRadians(parameters_.fieldOfViewInDegrees), parameters_.aspectRatio, parameters_.zNear, parameters_.zFar);
        math::Matrix4 viewProjection = projection * view;

        CameraData cameraData{
            .viewProjection = viewProjection
        };
        buffer->set(&cameraData, sizeof(cameraData), 0, true);
    }
}