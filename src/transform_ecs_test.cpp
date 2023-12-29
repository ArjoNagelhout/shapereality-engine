//
// Created by Arjo Nagelhout on 28/12/2023.
//

#include <iostream>
#include <thread>
#include <array>
#include <string>

#include "math/vector.h"
#include "math/vector.inl"
#include "math/matrix.h"
#include "math/matrix.inl"
#include "math/quaternion.h"
#include "math/quaternion.inl"

using namespace math;

struct Entity
{
	std::string name{};

	// components
	size_t transformComponentIndex{0};
};

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

struct Registry
{
	std::vector<Entity> entities{};
	std::vector<TransformComponent> transformComponents{};

	void createEntity()
	{
		entities.emplace_back(Entity{.name = "Entity"});
	}

	template<class Type>
	void registerComponent()
	{

	}
};
// https://github.com/skypjack/entt/commits/master/?after=f2c417435c021b6bd2e426ffd9d1fc116b804146+5000
// https://github.com/skypjack/entt/blob/ccf8f712ccbb32b2ac19e1b0f75ac0630db71caf/src/registry.hpp

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

		}
	}
}

int main(int argc, char* argv[])
{
	Registry registry;
	registry.createEntity();

	// create entities
	while (true)
	{
		update(registry);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}