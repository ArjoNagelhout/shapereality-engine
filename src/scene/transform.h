#ifndef BORED_ENGINE_TRANSFORM_H
#define BORED_ENGINE_TRANSFORM_H

#include <vector>

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

		void find(std::string const& objectName);
		void getChild(int const& index);
		void getLocalPositionAndRotation();
		void getPositionAndRotation();
		void getSiblingIndex();
		void inverseTransformDirection();
		void inverseTransformDirections();
		void inverseTransformPoint();
		void inverseTransformPoints();
		void inverseTransformVector();
		void inverseTransformVectors();
		[[nodiscard]] bool isChildOf(Transform* const parent);
		void lookAt(Transform* const target, );
		void rotate();
		void rotateAround();
		void setAsFirstSibling();
		void setAsLastSibling();
		void setLocalPositionAndRotation();
		void setParent();
		void setPositionAndRotation();
		void setSiblingIndex();
		void transformDirection();
		void transformDirections();
		void transformPoint();
		void transformPoints();
		void transformVector();
		void transformVectors();
		void translate();

	private:
		Transform* parent;
		std::vector<Transform*> children;

	};
}

#endif //BORED_ENGINE_TRANSFORM_H
