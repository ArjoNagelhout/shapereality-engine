#ifndef BORED_ENGINE_TRANSFORM_H
#define BORED_ENGINE_TRANSFORM_H

#include <vector>

#include "../math/vector.h"
#include "../types.h"
#include "../math/quaternion.h"
#include "../math/matrix.h"

#include "component.h"

using namespace math;
using namespace engine;

namespace scene
{
	class Transform : public Component
	{
	public:
		explicit Transform();
		~Transform();

		// properties
		int getChildCount();
		Vector3 getEulerAngles();
		void setEulerAngles(Vector3 const& eulerAngles);
		Vector3 getForward();
		Vector3 getLocalEulerAngles();
		void setLocalEulerAngles(Vector3 const& localEulerAngles);
		Vector3 getLocalPosition();
		void setLocalPosition(Vector3 const& localPosition);
		Quaternion getLocalRotation();
		void setLocalRotation(Quaternion const& localRotation);
		Vector3 getLocalScale();
		void setLocalScale(Vector3 const& localScale);
		Matrix4x4 getLocalToWorldMatrix();
		Vector3 getLossyScale();
		Transform* getParent(); // setParent already in public methods
		Vector3 getPosition();
		void setPosition(Vector3 const& position);
		Vector3 getRight();
		Transform* getRoot();
		Quaternion getRotation();
		void setRotation(Quaternion const& rotation);
		Vector3 getUp();
		Matrix4x4 getWorldToLocalMatrix();

		// public methods

		void detachChildren();
		Transform* find(std::string const& objectName);
		Transform* getChild(int const& index);
		void getLocalPositionAndRotation(Vector3& localPosition, Quaternion& localRotation); // out parameters
		void getPositionAndRotation(Vector3& position, Vector3& rotation);
		int getSiblingIndex();
		Vector3 inverseTransformDirection(Vector3 const& direction);
		Vector3 inverseTransformDirection(float const& x, float const& y, float const& z);
		void inverseTransformDirections(std::vector<Vector3>& directions); // in-place editing
		Vector3 inverseTransformPoint(Vector3 const& position);
		void inverseTransformPoints(std::vector<Vector3>& positions);
		Vector3 inverseTransformVector(Vector3 const& vector);
		void inverseTransformVectors(std::vector<Vector3>& vectors);
		[[nodiscard]] bool isChildOf(Transform* const parent);
		void lookAt(Transform* const target, Vector3 const& worldUp = Vector3{});
		void rotate(Vector3 const& eulers, Space relativeTo = Space::Self);
		void rotate(float const& xAngle, float const& yAngle, float const& zAngle, Space relativeTo = Space::Self);
		void rotate(Vector3 const& axis, float const& angle, Space relativeTo = Space::Self);
		void rotateAround(Vector3 const& point, Vector3 const& axis, float const& angle);
		void setAsFirstSibling();
		void setAsLastSibling();
		void setLocalPositionAndRotation(Vector3 const& localPosition, Quaternion const& localRotation);
		void setParent(Transform* parent, bool worldPositionStays = true);
		void setPositionAndRotation(Vector3 const& position, Quaternion const& rotation);
		void setSiblingIndex(int const& index);
		Vector3 transformDirection(Vector3 const& direction);
		Vector3 transformDirection(float const& x, float const& y, float const& z);
		void transformDirections(std::vector<Vector3>& directions); // in-place editing
		Vector3 transformPoint(Vector3 const& position);
		void transformPoints(std::vector<Vector3>& positions);
		Vector3 transformVector(Vector3 const& vector);
		void transformVectors(std::vector<Vector3>& vectors);
		void translate(Vector3 const& translation, Space relativeTo = Space::Self);

	private:
		Transform* parent;
		std::vector<Transform*> children;

		Vector3 localPosition;
		Quaternion localRotation;
		Vector3 localScale;
	};
}

#endif //BORED_ENGINE_TRANSFORM_H
