//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_SCENE_H
#define BORED_ENGINE_SCENE_H

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "../math/vector.h"
#include "../math/matrix.h"

namespace scene
{
	// The main advantage of using an ECS (Entity-component-system) approach
	// compared to a traditional object-oriented approach for representing
	// objects in the scene / experience, is that for behaviour that is shared
	// among a large amount of objects, a system can iterate over all of these
	// objects and apply the same calculations.
	//
	// It enables linear data access for a given component's data representation.
	//
	// object-oriented programming can still be used for more complex behaviour
	// and "managers"

	// https://gist.github.com/adamnemecek/ae2755c5c4eaabd0d864e6c62dbe5088

	using ComponentId = size_t;

	// an entity itself doesn't store any data, but only contains a list of component IDs it is
	/// connected to
	struct Entity
	{
		std::vector<ComponentId> componentIds;
	};

	// contains only data
	struct Component
	{

	};

	template<typename T>
	class System
	{
	public:
		void update();

	protected:
		// a system owns the components it iterates over
		std::vector<T> pComponents;
	};

	struct TransformComponent
	{

	};

	class TransformSystem final : public System<TransformComponent>
	{

	};

	// a scene contains both world-space 3D objects
	// as 2D UI to simplify development.
	// they share the same scene graph
	class Scene
	{
	public:
		explicit Scene();
		~Scene();

		void update(); // updates all systems in the scene
		void render(); // renders the scene and all its objects

	private:
		std::vector<std::unique_ptr<Entity>> pEntities{};

		//std::vector<> pSystems{};
	};
}

#endif //BORED_ENGINE_SCENE_H
