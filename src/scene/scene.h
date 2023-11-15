//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef BORED_ENGINE_SCENE_H
#define BORED_ENGINE_SCENE_H

#include "object.h"
#include "transform.h"

namespace scene
{
	// a scene contains both world-space 3D objects
	// as 2D UI to simplify development.
	// they share the same scene graph
	class Scene
	{
	public:
		explicit Scene();
		~Scene();

		void render(); // renders the scene and all its objects
		std::vector<Object*> getRootObjects();
		int getRootCount();
		std::string const getPath();

	private:
		std::vector<Object*> rootObjects;
		std::vector<std::unique_ptr<Object>> objects; // all objects of the scene stored flat
	};
}

#endif //BORED_ENGINE_SCENE_H
