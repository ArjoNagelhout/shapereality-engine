//
// Created by Arjo Nagelhout on 16/12/2023.
//

#include "camera.h"

#include <iostream>

#include "math/vector.h"
#include "math/vector.inl"
#include "math/matrix.inl"
#include "math/quaternion.inl"

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
		math::vec3 target = math::vec3::one;
		math::mat4 view = math::createLookAtMatrix(eye, target, math::vec3::up);

		math::mat4 translation = math::createTranslationMatrix(worldPosition);
		math::mat4 rotation = math::createRotationMatrix(math::Quaternion::identity);//math::Quaternion{0.1604506f, -0.1985467f, 0.03296775f, 0.9663063f});
		math::mat4 scale = math::createScaleMatrix(math::vec3{{1, 1, 1}});

		math::mat4 projection = math::createPerspectiveProjectionMatrix(fieldOfView, aspectRatio, zNear, zFar);

		//std::cout << view << std::endl;

		math::mat4 viewProjectionMatrix = projection * scale * rotation * translation;

		viewProjectionMatrix = viewProjectionMatrix.transpose();

		std::cout << eye << std::endl;
		std::cout << viewProjectionMatrix << std::endl;

		auto* pCameraData = reinterpret_cast<math::mat4*>(pBuffer->getContents());
		*pCameraData = viewProjectionMatrix;
		pBuffer->didModifyRange(graphics::Range{.offset = 0, .length = sizeof(math::mat4)});
	}
}