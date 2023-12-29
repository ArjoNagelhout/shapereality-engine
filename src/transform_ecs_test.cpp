//
// Created by Arjo Nagelhout on 28/12/2023.
//

#include <iostream>
#include <thread>
#include <array>
#include <string>
#include <limits>

#include "math/vector.h"
#include "math/vector.inl"
#include "math/matrix.h"
#include "math/matrix.inl"
#include "math/quaternion.h"
#include "math/quaternion.inl"

#include "entity/sparse_set.h"

using namespace math;
using namespace entity;

// https://github.com/skypjack/entt/commits/master/?after=f2c417435c021b6bd2e426ffd9d1fc116b804146+5000
// https://github.com/skypjack/entt/blob/ccf8f712ccbb32b2ac19e1b0f75ac0630db71caf/src/registry.hpp

// only contains data, no logic
struct TransformComponent
{
	vec3 localPosition{vec3::zero};
	Quaternion localRotation{Quaternion::identity};
	vec3 localScale{vec3::one};

	bool dirty{true};

	mat4 localToParentTransform{mat4::identity}; // from local space to parent space
	mat4 localToWorldTransform{
		mat4::identity}; // from local space to world space (with parent's transformations applied)

	size_t parentIndex{0};
};

// entt uses paging because this makes allocating more efficient.

struct Registry
{
	std::vector<entity_type> entities{};
	std::vector<TransformComponent> transformComponents{};

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
};

void update(Registry& registry)
{
	for (auto& t: registry.transformComponents)
	{
		if (t.dirty)
		{
			// update the localToParentTransform
			t.localToParentTransform = math::createTranslationRotationScaleMatrix(t.localPosition, t.localRotation,
																				  t.localScale);
			t.dirty = false;
		}

		// calculate the localToWorldTransform
		if (t.parentIndex > 0)
		{
			TransformComponent const& parent = registry.transformComponents[t.parentIndex];
			t.localToWorldTransform = parent.localToWorldTransform * t.localToParentTransform;
		}
	}

	std::cout << "updated" << std::endl;
}

int main(int argc, char* argv[])
{
	Registry registry;
	registry.createEntity();

	SparseSet<TransformComponent, 100> set;

	set.emplace(5);

	// create entities
	while (true)
	{
		update(registry);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
