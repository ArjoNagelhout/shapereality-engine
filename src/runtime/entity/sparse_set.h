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

    class SparseSetBase
    {
    public:
        virtual ~SparseSetBase() = default;
    };

    template<typename Type>
    class SparseSet final : public SparseSetBase
    {
    public:
        constexpr explicit SparseSet() = default;

        constexpr ~SparseSet() override = default;

        // returns whether the set contains an item at the given index
        [[nodiscard]] bool contains(size_type index) const
        {
            if (index >= sparse.size())
            {
                return false;
            }

            return sparse[index] != TOMBSTONE;
        }

        // emplace a value in the set at the given index
        // returns whether emplacing was successful
        template<typename... Args>
        [[nodiscard]] bool emplace(size_type index, Args&&... args)
        {
            if (contains(index))
            {
                return false;
            }

            // ensure index is not larger than the max size
            if ((index + 1) >= MAX_SIZE)
            {
                return false;
            }

            // emplace value in dense array
            denseValues.emplace_back(args...);
            dense.emplace_back(index); // set sparse index in dense array

            // set dense index in sparse array
            size_type denseIndex = dense.size() - 1;
            sparse[index] = denseIndex;

            return true;
        }

        // remove an element from the set at the given index
        // returns whether the removal was successful
        [[nodiscard]] bool remove(size_type index)
        {
            if (!contains(index))
            {
                return false;
            }

            size_type const denseIndex = sparse[index];
            std::swap(denseValues.begin() + denseIndex, denseValues.back());

            size_type const swappedSparseIndex = dense.back();
            dense[denseIndex] = swappedSparseIndex;
            sparse[swappedSparseIndex] = denseIndex;

            // pop the last element in dense array
            dense.pop_back();
            denseValues.pop_back();

            // set sparse to null
            sparse[index] = TOMBSTONE;
        }

    private:
        std::vector<size_type> sparse; // contains indices to dense array
        std::vector<size_type> dense; // contains indices to sparse array
        std::vector<Type> denseValues; // contains values (ordered 1:1 with dense array)
    };
}

#endif //SHAPEREALITY_SPARSE_SET_H
