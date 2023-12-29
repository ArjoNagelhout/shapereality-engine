//
// Created by Arjo Nagelhout on 30/12/2023.
//

#ifndef SHAPEREALITY_SPARSE_SET_H
#define SHAPEREALITY_SPARSE_SET_H

#include <vector>
#include <array>

#include "config.h"

namespace entity
{
	// an "empty" value in the sparse set
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
	template<typename Type, size_type Size>
	requires (Size <= MAX_SIZE)
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
		[[maybe_unused]] constexpr bool emplace(size_type index, Args&& ... args)
		{
			if (!inRange(index))
			{
				return false;
			}

			if (contains(index))
			{
				return false;
			}

			dense.emplace_back(std::forward<Args>(args)...);

			// set sparse index
			size_type size = dense.size();
			sparse[index] = size - 1;

			return true;
		}

		[[nodiscard]] constexpr bool inRange(size_type index) const
		{
			return (index <= Size - 1);
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
		[[maybe_unused]] constexpr bool remove(size_type index)
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
		[[nodiscard]] constexpr Type& get(size_type index)
		{
			return dense[sparse[index]];
		}

	private:
		std::array<size_type, Size> sparse;
		std::vector<Type> dense;
	};
}

#endif //SHAPEREALITY_SPARSE_SET_H
