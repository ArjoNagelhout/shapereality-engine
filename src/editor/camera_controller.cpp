//
// Created by Arjo Nagelhout on 24/04/2024.
//

#include "camera_controller.h"

#include <math/utils.h>
#include <math/quaternion.inl>

namespace editor
{
    CameraController::CameraController(renderer::Camera& camera_, input::Input& input_, CameraControllerParameters parameters)
        : camera(camera_), input(input_), parameters_(parameters)
    {

    }

    CameraController::~CameraController() = default;

    void CameraController::update()
    {
        // update position
        auto const dx = static_cast<float>(input.getKey(graphics::KeyCode::D) - input.getKey(graphics::KeyCode::A));
        auto const dy = static_cast<float>(input.getKey(graphics::KeyCode::E) - input.getKey(graphics::KeyCode::Q));
        auto const dz = static_cast<float>(input.getKey(graphics::KeyCode::W) - input.getKey(graphics::KeyCode::S));
        position += math::Vector3{{dx, dy, dz}} * parameters_.speed;

        //  update rotation
        auto const dh = static_cast<float>(input.getKey(graphics::KeyCode::RightArrow) - input.getKey(graphics::KeyCode::LeftArrow));
        auto const dv = static_cast<float>(input.getKey(graphics::KeyCode::UpArrow) - input.getKey(graphics::KeyCode::DownArrow));
        horizontalRotation += dh * parameters_.rotationSpeed;
        verticalRotation += dv * parameters_.rotationSpeed;

        // construct matrix
        math::Quaternion rotation = math::Quaternion::createFromEulerInRadians(
            math::Vector3{
                {math::degreesToRadians(verticalRotation), math::degreesToRadians(horizontalRotation), 0}
            }
        );

        math::Matrix4 transform = math::createTranslationRotationScaleMatrix(
            position, rotation, math::Vector3::one
        );

        camera.setTransform(transform);
    }
}