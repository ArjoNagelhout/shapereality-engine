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
		graphics::BufferDescriptor descriptor{
			.type = graphics::BufferDescriptor::Type::Uniform,
			.storageMode = graphics::BufferDescriptor::StorageMode::Managed,
			.data = nullptr,
			.length = sizeof(math::mat4),
			.stride = sizeof(math::mat4)
		};

		pBuffer = pDevice->createBuffer(descriptor);

		updateCameraDataBuffer();
	}

	Camera::~Camera() = default;

	CameraProjection Camera::getCameraProjection() const
	{
		return cameraProjection;
	}

	void Camera::setCameraProjection(CameraProjection _cameraProjection)
	{
		cameraProjection = _cameraProjection;
		updateCameraDataBuffer();
	}

	float Camera::getAspectRatio() const
	{
		return aspectRatio;
	}

	void Camera::setAspectRatio(float _aspectRatio)
	{
		aspectRatio = _aspectRatio;
		updateCameraDataBuffer();
	}

	float Camera::getFieldOfView() const
	{
		return fieldOfView;
	}

	void Camera::setFieldOfView(float _fieldOfView)
	{
		fieldOfView = _fieldOfView;
		updateCameraDataBuffer();
	}

	void Camera::setWorldPosition(math::vec3 position)
	{
		worldPosition = position;
		updateCameraDataBuffer();
	}

	graphics::IBuffer* Camera::getCameraDataBuffer() const
	{
		return pBuffer.get();
	}

	void Camera::updateCameraDataBuffer()
	{
		math::vec3 eye = worldPosition;
		math::vec3 target = math::vec3::zero;
		math::mat4 view = math::createLookAtMatrix(eye, target, math::vec3::up);

		math::mat4 projection = math::createPerspectiveProjectionMatrix(fieldOfView, aspectRatio, zNear, zFar);

		math::mat4 viewProjectionMatrix = view * projection;

		viewProjectionMatrix = viewProjectionMatrix.transpose();

		auto* pCameraData = reinterpret_cast<math::mat4*>(pBuffer->getContents());
		*pCameraData = viewProjectionMatrix;
		pBuffer->didModifyRange(graphics::Range{.offset = 0, .length = sizeof(math::mat4)});
	}
}