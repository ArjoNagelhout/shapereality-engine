#ifndef BORED_ENGINE_TRANSFORM_H
#define BORED_ENGINE_TRANSFORM_H

#include <vector>

#include "../math/vector.h"
#include "../types.h"
#include "../math/quaternion.h"

using namespace math;
using namespace engine;

namespace scene
{
	class Transform
	{
	public:
		explicit Transform();
		~Transform();

		Transform* getParent();
		std::vector<Transform*>& getChildren();
		void getObject();

		std::string const getName();
		void setName(std::string const& name);

		void detachChildren();
		void find(std::string const& objectName);
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
		void lookAt(Transform* const target, Vector3 const& worldUp = Vector3::up);
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
	};
}

#endif //BORED_ENGINE_TRANSFORM_H
