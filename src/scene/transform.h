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
		vec3 getEulerAngles();
		void setEulerAngles(vec3 const& eulerAngles);
		vec3 getForward();
		vec3 getLocalEulerAngles();
		void setLocalEulerAngles(vec3 const& localEulerAngles);
		vec3 getLocalPosition();
		void setLocalPosition(vec3 const& localPosition);
		Quaternion getLocalRotation();
		void setLocalRotation(Quaternion const& localRotation);
		vec3 getLocalScale();
		void setLocalScale(vec3 const& localScale);
		mat4 getLocalToWorldMatrix();
		vec3 getLossyScale();
		Transform* getParent(); // setParent already in public methods
		vec3 getPosition();
		void setPosition(vec3 const& position);
		vec3 getRight();
		Transform* getRoot();
		Quaternion getRotation();
		void setRotation(Quaternion const& rotation);
		vec3 getUp();
		mat4 getWorldToLocalMatrix();

		// public methods

		void detachChildren();
		Transform* find(std::string const& objectName);
		Transform* getChild(int const& index);
		void getLocalPositionAndRotation(vec3& localPosition, Quaternion& localRotation); // out parameters
		void getPositionAndRotation(vec3& position, vec3& rotation);
		int getSiblingIndex();
		vec3 inverseTransformDirection(vec3 const& direction);
		vec3 inverseTransformDirection(float const& x, float const& y, float const& z);
		void inverseTransformDirections(std::vector<vec3>& directions); // in-place editing
		vec3 inverseTransformPoint(vec3 const& position);
		void inverseTransformPoints(std::vector<vec3>& positions);
		vec3 inverseTransformVector(vec3 const& vector);
		void inverseTransformVectors(std::vector<vec3>& vectors);
		[[nodiscard]] bool isChildOf(Transform* const parent);
		void lookAt(Transform* const target, vec3 const& worldUp = vec3{});
		void rotate(vec3 const& eulers, Space relativeTo = Space::Self);
		void rotate(float const& xAngle, float const& yAngle, float const& zAngle, Space relativeTo = Space::Self);
		void rotate(vec3 const& axis, float const& angle, Space relativeTo = Space::Self);
		void rotateAround(vec3 const& point, vec3 const& axis, float const& angle);
		void setAsFirstSibling();
		void setAsLastSibling();
		void setLocalPositionAndRotation(vec3 const& localPosition, Quaternion const& localRotation);
		void setParent(Transform* parent, bool worldPositionStays = true);
		void setPositionAndRotation(vec3 const& position, Quaternion const& rotation);
		void setSiblingIndex(int const& index);
		vec3 transformDirection(vec3 const& direction);
		vec3 transformDirection(float const& x, float const& y, float const& z);
		void transformDirections(std::vector<vec3>& directions); // in-place editing
		vec3 transformPoint(vec3 const& position);
		void transformPoints(std::vector<vec3>& positions);
		vec3 transformVector(vec3 const& vector);
		void transformVectors(std::vector<vec3>& vectors);
		void translate(vec3 const& translation, Space relativeTo = Space::Self);

	private:
		Transform* parent;
		std::vector<Transform*> children;

		vec3 localPosition;
		Quaternion localRotation;
		vec3 localScale;
	};
}

#endif //BORED_ENGINE_TRANSFORM_H
