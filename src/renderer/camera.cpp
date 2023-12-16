//
// Created by Arjo Nagelhout on 16/12/2023.
//

#include "camera.h"

#include "math/vector.h"
#include "math/vector.inl"
#include "math/matrix.inl"

namespace renderer
{
	Camera::Camera(graphics::IDevice* pDevice)
	{
		math::vec3 eye = math::vec3{{0, 1, 1}};
		math::vec3 target = math::vec3::zero;
		math::mat4 view = math::createLookAtMatrix(eye, target, math::vec3::up);

		math::mat4 projection = math::createPerspectiveProjectionMatrix(fieldOfView, aspectRatio, zNear, zFar);

		math::mat4 viewProjectionMatrix = view * projection;

		graphics::BufferDescriptor descriptor{
			.type = graphics::BufferDescriptor::Type::Uniform,
			.storageMode = graphics::BufferDescriptor::StorageMode::Managed,
			.data = &viewProjectionMatrix,
			.length = sizeof(viewProjectionMatrix),
			.stride = sizeof(math::mat4)
		};

		pBuffer = pDevice->createBuffer(descriptor);
	}

	Camera::~Camera() = default;

	CameraProjection Camera::getCameraProjection() const
	{
		return cameraProjection;
	}

	void Camera::setCameraProjection(CameraProjection _cameraProjection)
	{
		cameraProjection = _cameraProjection;
	}

	float Camera::getAspectRatio() const
	{
		return aspectRatio;
	}

	void Camera::setAspectRatio(float _aspectRatio)
	{
		aspectRatio = _aspectRatio;
	}

	float Camera::getFieldOfView() const
	{
		return fieldOfView;
	}

	void Camera::setFieldOfView(float _fieldOfView)
	{
		fieldOfView = _fieldOfView;
	}

	graphics::IBuffer* Camera::getCameraDataBuffer() const
	{
		return pBuffer.get();
	}
}