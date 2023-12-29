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

using namespace math;

using size_type = size_t;
using entity_type = size_type;

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

// an "empty" value in the
constexpr size_type TOMBSTONE = std::numeric_limits<size_t>::max();

// max size is always +1 compared to max index, but here we want to limit
// to one less than tombstone. So + 1 - 1 cancel each other out.
constexpr size_type MAX_SIZE = TOMBSTONE;

/*
 * A sparse set contains a level of indirection, where you access the contents of
 * the set through an array with indexes. This index can be used to retrieve the actual
 * value from the dense storage.
 *
 * Otherwise, each entity would have to store which component indices it uses.
 * Or we would preallocate the maximum amount of components in an array, which is silly
 *
 * entity index -> component index -> component:
 *
 * dense[sparse[index]]
 */
template<typename Type, size_type Size> requires (Size <= MAX_SIZE)
struct SparseSet
{
	constexpr explicit SparseSet()
	{
		sparse.fill(TOMBSTONE);
	}

	constexpr ~SparseSet() = default;

	/**
	 * @return whether emplacing was successful
	 */
	template<typename... Args>
	[[maybe_unused]] constexpr bool emplace(size_type index, Args&&... args)
	{
		if (!inRange(index))
		{
			return;
		}

		if (contains(index))
		{
			return;
		}

		dense.emplace_back(std::forward<Args>(args)...);

		// set sparse index
		size_type size = dense.size();
		sparse[index] = size - 1;
	}

	[[nodiscard]] constexpr bool inRange(size_type index) const
	{
		return (index <= Size-1);
	}

	[[nodiscard]] constexpr bool contains(size_type index) const
	{
		if (!inRange(index))
		{
			return false;
		}

		return (sparse[index] != TOMBSTONE);
	}

	/**
	 * @param index
	 * @return whether the removal was successful
	 */
	[[maybe_unused]] bool remove(size_type index)
	{
		if (!contains(index))
		{
			return false;
		}

		dense.erase(dense.begin() + sparse[index]);
		sparse[index] = TOMBSTONE;
		return true;
	}

	// does not throw an error, use contains before using
	Type& get(size_type index)
	{
		return dense[sparse[index]];
	}

	// functionality:
	// insertion [x]
	// deletion [x]
	// contains [x]
	// get [x]
	// sort?

private:
	std::array<size_type, Size> sparse;
	std::vector<Type> dense;
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
