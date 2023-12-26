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

	// a camera is a simple piece of data that does not do anything intelligent,
	// nor does it exist in a hierarchy. This makes refactoring the transform hierarchy easier
	//
	// it only generates a frustum from the set properties
	class Camera
	{
	public:
		explicit Camera(graphics::IDevice* pDevice);
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
		void setTransform(math::mat4 const& _transform);

		// get camera data buffer (not const, because it updates the buffer if it was dirtied)
		[[nodiscard]] graphics::IBuffer* getCameraDataBuffer();

	private:
		// transform
		math::mat4 transform = math::mat4::identity;

		// projection
		CameraProjection cameraProjection{CameraProjection::Perspective};
		float aspectRatio{1.0f}; // width / height
		float fieldOfViewInDegrees{60.0f};
		float zNear{0.1f};
		float zFar{1000.0f};

		// data

		// if the buffer is requested, it will update the buffer if it was dirtied
		// this is to ensure that when setting 5 properties, it doesn't recalculate
		// the camera data 5 times.
		bool dirty{true};
		struct CameraData
		{
			math::mat4 viewProjection;
		};
		std::unique_ptr<graphics::IBuffer> pBuffer;

		void updateBuffer();
	};
}

#endif //SHAPEREALITY_CAMERA_H
