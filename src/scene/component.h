#ifndef BORED_ENGINE_COMPONENT_H
#define BORED_ENGINE_COMPONENT_H

namespace scene
{
	class Component
	{
	public:
		explicit Component();
		virtual ~Component();

		void getComponent();
		void getComponentInChildren();
		void getComponentInParent();
		void getComponents();
		void getComponentsInChildren();
		void getComponentsInParent();
		void tryGetComponent();

	protected:

	private:

	};
}

#endif //BORED_ENGINE_COMPONENT_H
