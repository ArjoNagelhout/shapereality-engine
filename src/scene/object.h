#ifndef BORED_ENGINE_OBJECT_H
#define BORED_ENGINE_OBJECT_H

#include <string>

namespace scene
{
	class Transform;

	class Object
	{
	public:
		explicit Object();
		~Object();

		// properties
		Object* getObject();
		std::string getTag();
		void setTag(std::string const& tag);
		Transform* getTransform();
		int getHideFlags();
		void setHideFlags(int hideFlags);
		std::string getName();
		void setName(std::string const& name);

		// public methods
		void getComponent();
		void getComponentInChildren();
		void getComponentInParent();
		void getComponents();
		void getComponentsInChildren();
		void getComponentsInParent();
		void tryGetComponent();

		// static methods

		static void destroy();
		static void destroyImmediate();
		static void dontDestroyOnLoad();
		static void findAnyObjectByType();
		static void findFirstObjectByType();
		static void findObjectOfType();
		static void findObjectsByType();
		static void findObjectsOfType();
		static void instantiate();

	private:
		Transform* transform;
		std::string name;
	};
}

#endif //BORED_ENGINE_OBJECT_H
