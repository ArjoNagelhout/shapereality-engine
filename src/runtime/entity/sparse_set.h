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

        // returns whether the set contains an item at the given index
        [[nodiscard]] bool contains(size_type index) const
        {
            if (index >= sparse.size())
            {
                return false;
            }

            return sparse[index] != TOMBSTONE;
        }

        // get the size of the sparse array
        [[nodiscard]] size_type size() const
        {
            return sparse.size();
        }

        // resizes the sparse array
        bool resize(size_type size)
        {
            if (size >= MAX_SIZE)
            {
                return false;
            }

            if (size > sparse.size())
            {
                // resize and initialize with null indices
                sparse.resize(size, TOMBSTONE);
            }
            else if (size < sparse.size())
            {
                // remove all relevant elements from dense array
                for (size_type i = (sparse.size() - 1); i > (size - 1); i--)
                {
                    remove(i);
                }

                // resize the sparse array
                sparse.resize(size);
            }

            return true;
        }

        // remove an element from the set at the given index
        // returns whether the removal was successful
        bool remove(size_type index)
        {
            if (!contains(index))
            {
                return false;
            }

            size_type const denseIndex = sparse[index];
            size_type const swappedSparseIndex = dense.back();
            dense[denseIndex] = swappedSparseIndex;
            sparse[swappedSparseIndex] = denseIndex;

            // pop the last element in dense array
            dense.pop_back();

            // set sparse to null
            sparse[index] = TOMBSTONE;

            swapAndPop(denseIndex);
        }

    protected:
        virtual void swapAndPop(size_type const denseIndex) = 0;

        std::vector<size_type> sparse; // contains indices to dense array
        std::vector<size_type> dense; // contains indices to sparse array
    };

    template<typename Type>
    class SparseSet final : public SparseSetBase
    {
    public:
        explicit SparseSet() = default;

        ~SparseSet() override = default;

        // emplace a value in the set at the given index
        // returns whether emplacing was successful
        template<typename... Args>
        bool emplace(size_type index, Args&&... args)
        {
            // ensure index is not larger than the max size
            if ((index + 1) >= MAX_SIZE)
            {
                return false;
            }

            if (contains(index))
            {
                return false;
            }

            // e.g. index = 3, size = 3. means we need to resize to size = 4
            if (index >= sparse.size())
            {
                // resize and initialize with null indices
                sparse.resize(index + 1, TOMBSTONE);
            }

            dense.emplace_back(index); // set sparse index in dense array

            // set dense index in sparse array
            size_type denseIndex = dense.size() - 1;
            sparse[index] = denseIndex;

            // emplace value in dense array
            denseValues.emplace_back(args...);

            return true;
        }

        void swapAndPop(const entity::size_type denseIndex) override
        {
            // swap and pop dense values
            std::swap(denseValues[denseIndex], denseValues.back());
            denseValues.pop_back();
        }

    private:
        std::vector<Type> denseValues; // contains values (ordered 1:1 with dense array)
    };
}

#endif //SHAPEREALITY_SPARSE_SET_H
