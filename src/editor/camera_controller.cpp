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
        auto const dyaw = static_cast<float>(input.getKey(graphics::KeyCode::RightArrow) - input.getKey(graphics::KeyCode::LeftArrow));
        auto const dpitch = static_cast<float>(input.getKey(graphics::KeyCode::UpArrow) - input.getKey(graphics::KeyCode::DownArrow));
        auto const droll = static_cast<float>(input.getKey(graphics::KeyCode::RightBracket) - input.getKey(graphics::KeyCode::LeftBracket));
        targetYaw += dyaw * parameters_.rotationSpeed;
        targetPitch += dpitch * parameters_.rotationSpeed;
        targetRoll += droll * parameters_.rotationSpeed;

        currentYaw = math::lerp(currentYaw, targetYaw, parameters_.rotationLerpSpeed);
        currentPitch = math::lerp(currentPitch, targetPitch, parameters_.rotationLerpSpeed);
        currentRoll = math::lerp(currentRoll, targetRoll, parameters_.rotationLerpSpeed);

        // construct matrix
        math::Quaternionf pitch = math::Quaternionf::angleAxis(math::degreesToRadians(currentPitch), math::Vector3::right);
        math::Quaternionf yaw = math::Quaternionf::angleAxis(math::degreesToRadians(currentYaw), math::Vector3::up);
        math::Quaternionf roll = math::Quaternionf::angleAxis(math::degreesToRadians(currentRoll), math::Vector3::forward);

        math::Quaternionf rotation = yaw * pitch * roll;

        targetPosition += rotation * delta;

        currentPosition = math::Vector3::lerp(currentPosition, targetPosition, parameters_.lerpSpeed);

        camera.position() = currentPosition;
        camera.rotation() = rotation;
    }
}