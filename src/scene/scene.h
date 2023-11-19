//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_SCENE_H
#define BORED_ENGINE_SCENE_H

#include "object.h"
#include "transform.h"

namespace scene
{
	using ObjectId = uint32_t;

	// a scene contains both world-space 3D objects
	// as 2D UI to simplify development.
	// they share the same scene graph
	class Scene
	{
	public:
		explicit Scene();
		~Scene();

		void render(); // renders the scene and all its objects

		void addObject(std::unique_ptr<Object> const& object);
		void removeObject(Object* object);

	private:
		std::unordered_map<ObjectId, std::unique_ptr<Object>> pObjects{}; // all objects of the scene stored flat

		std::vector<ObjectId> pRootObjects{};
		std::vector<ObjectId> pObjectsSortedByShader{}; // sorted by material type for submitting to the renderer
	};
}

#endif //BORED_ENGINE_SCENE_H
