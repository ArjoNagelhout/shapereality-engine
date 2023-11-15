#ifndef BORED_ENGINE_OBJECT_H
#define BORED_ENGINE_OBJECT_H

namespace scene
{
	class Transform;

	class Object
	{
	public:
		explicit Object();
		~Object();

		void Start();
		void Update();
		void OnDestroy();
		void Destroy();

		Transform* getTransform();

	private:
		Transform* transform;
	};
}

#endif //BORED_ENGINE_OBJECT_H
