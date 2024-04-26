//
// Created by Arjo Nagelhout on 24/04/2024.
//

#ifndef SHAPEREALITY_CAMERA_CONTROLLER_H
#define SHAPEREALITY_CAMERA_CONTROLLER_H

#include <renderer/camera.h>
#include <input/input.h>
#include <math/vector.h>

namespace editor
{
    // configuration for camera controller
    struct CameraControllerParameters
    {
        float speed = 2.0f;
        float rotationSpeed = 2.0f;
        float lerpSpeed = 0.1f;
        float rotationLerpSpeed = 0.2f;
        float shiftSpeedMultiplier = 3.0f;
        math::Vector3 startPosition{26, 246.84, 56.681};
    };

    class CameraController final
    {
    public:
        explicit CameraController(renderer::Camera& camera, input::Input& input, CameraControllerParameters parameters = {});

        ~CameraController();

        // updates the camera based on key presses
        void update();

        [[nodiscard]] CameraControllerParameters& parameters();

    private:
        renderer::Camera& camera;
        input::Input& input;
        CameraControllerParameters parameters_;

        math::Vector3 currentPosition = math::Vector3::zero;
        math::Vector3 targetPosition = math::Vector3::zero;
        float currentHorizontalRotation = 0.0f;
        float targetHorizontalRotation = 0.0f;
        float currentVerticalRotation = 90.0f;
        float targetVerticalRotation = 90.0f;
    };
}

#endif //SHAPEREALITY_CAMERA_CONTROLLER_H
