//
// Created by Arjo Nagelhout on 24/04/2024.
//

#include "camera_controller.h"

#include <math/utility.h>
#include <math/quaternion.inl>
#include <utility>

#include <iostream>

namespace editor
{
    CameraController::CameraController(renderer::Camera& camera_, input::Input& input_, CameraControllerParameters parameters)
        : camera(camera_), input(input_), parameters_(std::move(parameters))
    {
        currentPosition = parameters_.startPosition;
        targetPosition = currentPosition;
    }

    CameraController::~CameraController() = default;

    void CameraController::update()
    {
        // update position
        auto const dx = static_cast<float>(input.getKey(graphics::KeyCode::D) - input.getKey(graphics::KeyCode::A));
        auto const dy = static_cast<float>(input.getKey(graphics::KeyCode::E) - input.getKey(graphics::KeyCode::Q));
        auto const dz = static_cast<float>(input.getKey(graphics::KeyCode::W) - input.getKey(graphics::KeyCode::S));
        math::Vector3 delta{dx, dy, dz};

        float speed = parameters_.speed;
        if (input.getModifier(graphics::KeyboardModifier_Shift))
        {
            speed *= parameters_.shiftSpeedMultiplier;
        }

        delta *= speed;

        //  update rotation
        auto const dh = static_cast<float>(input.getKey(graphics::KeyCode::RightArrow) - input.getKey(graphics::KeyCode::LeftArrow));
        auto const dv = static_cast<float>(input.getKey(graphics::KeyCode::UpArrow) - input.getKey(graphics::KeyCode::DownArrow));
        targetHorizontalRotation += dh * parameters_.rotationSpeed;
        targetVerticalRotation += dv * parameters_.rotationSpeed;

        currentHorizontalRotation = math::lerp(currentHorizontalRotation, targetHorizontalRotation, parameters_.rotationLerpSpeed);
        currentVerticalRotation = math::lerp(currentVerticalRotation, targetVerticalRotation, parameters_.rotationLerpSpeed);

        // construct matrix
        math::Quaternionf h = math::Quaternionf::createFromEulerInRadians(
            math::Vector3{0, 0, math::degreesToRadians(currentHorizontalRotation)}
        );

        math::Quaternionf v = math::Quaternionf::createFromEulerInRadians(
            math::Vector3{0, math::degreesToRadians(currentVerticalRotation), 0}
        );

        math::Quaternionf rotation = v * h;

        targetPosition += rotation * delta;

        currentPosition = math::Vector3::lerp(currentPosition, targetPosition, parameters_.lerpSpeed);

        math::Matrix4 transform = math::createTRSMatrix(
            currentPosition, rotation, math::Vector3{1, 1, 1}
        );

        camera.setTransform(transform);

//        std::cout << targetPosition[0] << ", " << targetPosition[1] << ", " << targetPosition[2] << std::endl;
    }
}