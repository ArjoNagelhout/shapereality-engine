//
// Created by Arjo Nagelhout on 16/12/2023.
//

#ifndef BORED_ENGINE_CAMERA_H
#define BORED_ENGINE_CAMERA_H

#include "math/matrix.h"

namespace renderer
{
	enum class CameraProjection
	{
		Perspective,
		Orthographic,
	};

	// a camera is a simple piece of data that does not do anything intelligent,
	// nor does it exist in a hierarchy. This makes refactoring the transform hierarchy easier
	//
	// it only generates a frustum from the set properties
	class Camera
	{
	public:
		explicit Camera();
		~Camera();

		//
		[[nodiscard]] CameraProjection getCameraProjection() const;

		//
		void setCameraProjection(CameraProjection _cameraProjection);

		//
		[[nodiscard]] float getAspectRatio() const;

		//
		void setAspectRatio(float _aspectRatio);

		//
		[[nodiscard]] float getFieldOfView() const;

		//
		void setFieldOfView(float _fieldOfView);

		//
		[[nodiscard]] math::mat4 getFrustum() const;

	private:
		// input
		math::mat4 worldSpaceTransform = math::mat4::identity;
		CameraProjection cameraProjection{CameraProjection::Perspective};
		float aspectRatio{1.0f}; // width / height
		float fieldOfView{60}; // in degrees

		// output
		math::mat4 cachedFrustum{};
	};
}

#endif //BORED_ENGINE_CAMERA_H
