//
// Created by Arjo Nagelhout on 16/12/2023.
//

#include "camera.h"

#include <iostream>

#include "math/vector.h"
#include "math/vector.inl"
#include "math/matrix.inl"
#include "math/quaternion.inl"
#include "math/utils.h"

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

		math::mat4 trs = translation.inverse();

		//std::cout << "translation: " << translation << std::endl;
		//std::cout << "trs: " << trs << std::endl;


		std::cout << "aspectRatio: " << aspectRatio << std::endl;

		float const tanHalfFovY = tan(fieldOfView / 2.0f);

		std::cout << "tanHalfFovY: " << tanHalfFovY << std::endl;


		math::mat4 projection = math::createPerspectiveProjectionMatrix(math::degreesToRadians(fieldOfView), aspectRatio, zNear, zFar);


		math::mat4 viewProjectionMatrix = projection * trs;

		math::vec3 pos = math::vec3::zero;
		math::mat4 result = viewProjectionMatrix * math::createTranslationMatrix(pos);
		math::vec3 resultPos = math::vec3{{result(0, 3), result(1, 3), result(2, 3)}};

		std::cout << "worldPos: " << worldPosition << std::endl;
		std::cout << "translation: " << translation << std::endl;
		std::cout << "trs: " << trs << std::endl;
		std::cout << "projection: " << projection << std::endl;
		std::cout << "viewProjectionMatrix: " << viewProjectionMatrix << std::endl;
		std::cout << "resultPos: " << resultPos << std::endl;

		viewProjectionMatrix = viewProjectionMatrix.transpose();

		//std::cout << eye << std::endl;
		//std::cout << viewProjectionMatrix << std::endl;

		auto* pCameraData = reinterpret_cast<math::mat4*>(pBuffer->getContents());
		*pCameraData = viewProjectionMatrix;
		pBuffer->didModifyRange(graphics::Range{.offset = 0, .length = sizeof(math::mat4)});
	}
}