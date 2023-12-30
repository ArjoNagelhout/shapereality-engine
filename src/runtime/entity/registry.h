//
// Created by Arjo Nagelhout on 30/12/2023.
//

#ifndef SHAPEREALITY_REGISTRY_H
#define SHAPEREALITY_REGISTRY_H

#include "config.h"

#include <vector>

namespace entity
{
	// https://stackoverflow.com/questions/21269083/how-to-create-a-multiple-typed-object-pool-in-c

	class Registry
	{
	public:
		void createEntity()
		{
			entities.emplace_back();
		}

		// also destroys its components
		void destroyEntity()
		{

		}

		/**
		 *
		 * @tparam Type the type of the component
		 * @param entity the entity to add the component to
		 */
		template<typename Type>
		void addComponent(entity_type entity)
		{
			// check if the sparse set for this component was already created

		}

		void destroyComponent()
		{

		}

		/**
		 *
		 * @tparam Type type of the component
		 */
		template<typename Type>
		void hasComponent(entity_type entity)
		{

		}

	private:
		std::vector<entity_type> entities;
		std::unordered_map<type_id, std::unique_ptr<SparseSetBase>> components;
	};
}

#endif //SHAPEREALITY_REGISTRY_H
