//
// Created by Arjo Nagelhout on 16/12/2023.
//

#ifndef SHAPEREALITY_CAMERA_H
#define SHAPEREALITY_CAMERA_H

#include "math/matrix.h"
#include "math/matrix.inl"
#include "math/vector.h"

#include "graphics/device.h"
#include "graphics/buffer.h"

namespace renderer
{
    enum class CameraProjection
    {
        Perspective,
        Orthographic,
    };

    struct CameraParameters
    {
        float aspectRatio = 1.0f; // width / height
        CameraProjection projection = CameraProjection::Perspective;
        float fieldOfViewInDegrees = 60.0f;
        float zNear = 0.1f;
        float zFar = 1000.0f;
    };

    class Camera
    {
    public:
        explicit Camera(graphics::IDevice* device, CameraParameters parameters = {});

        ~Camera();

        // get camera data buffer (not const, because it updates the buffer if it was dirtied)
        [[nodiscard]] graphics::Buffer* getCameraDataBuffer();

        [[nodiscard]] CameraParameters& parameters();

        [[nodiscard]] math::Vector3f& position();

        [[nodiscard]] math::Quaternionf& rotation();

    private:
        struct CameraData
        {
            math::Matrix4 viewProjection;
        };

        CameraParameters parameters_;
        math::Vector3f position_ = math::Vector3f::zero;
        math::Quaternionf rotation_ = math::Quaternionf::identity;
        std::unique_ptr<graphics::Buffer> buffer;

        void updateBuffer();
    };
}

#endif //SHAPEREALITY_CAMERA_H
