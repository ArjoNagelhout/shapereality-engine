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
        float speed = 1.0f;
        float rotationSpeed = 1.0f;
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

        math::Vector3 position = math::Vector3::zero;
        float horizontalRotation = 0.0f;
        float verticalRotation = 0.0f;
    };
}

#endif //SHAPEREALITY_CAMERA_CONTROLLER_H
