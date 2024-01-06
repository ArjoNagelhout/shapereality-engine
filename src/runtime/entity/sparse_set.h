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

    // an iterator to iterate over a SparseSet
    //
    // note: internally, the iteration is done from the end of the dense array
    // to the beginning of the dense array (so in reverse order).
    //
    // this is because when removing an item, it gets swapped with the last item,
    // therefore, we would have to make sure that item gets iterated over as well,
    // even though we have already passed it.
    //
    // furthermore, when adding an item, we need to exclude it from iteration,
    // thus adding an extra check
    //
    // reverse iteration solves these issues.
    template<typename Type>
    struct SparseSetIterator final
    {
        using difference_type = std::ptrdiff_t;

        constexpr explicit SparseSetIterator(std::vector<Type>* dense, difference_type offset)
            : dense(dense), offset(offset)
        {
        }

        constexpr SparseSetIterator& operator++() noexcept
        {
            return --offset, *this;;
        }

        constexpr SparseSetIterator operator++(int) noexcept
        {
            SparseSetIterator original = *this;
            ++(*this);
            return original;
        }

        constexpr SparseSetIterator& operator--() noexcept
        {
            return ++offset, *this;
        }

        constexpr SparseSetIterator operator--(int) noexcept
        {
            SparseSetIterator original = *this;
            --(*this);
            return original;
        }

        constexpr SparseSetIterator& operator+=(difference_type const value) noexcept
        {
            offset -= value; // see note above, we use reverse iteration
            return *this;
        }

        constexpr SparseSetIterator operator+(difference_type const value) const noexcept
        {
            SparseSetIterator copy = *this;
            return (copy += value);
        }

        constexpr SparseSetIterator& operator-=(difference_type const value) noexcept
        {
            return (*this += -value);
        }

        constexpr SparseSetIterator operator-(difference_type const value) const noexcept
        {
            return (*this + -value);
        }

        // returns a pointer (standard for iterators)
        [[nodiscard]] constexpr Type* operator->() noexcept
        {
            return dense->data() + index();
        }

        // returns reference (standard for iterators)
        [[nodiscard]] constexpr Type& operator*() noexcept
        {
            return *operator->();
        }

        [[nodiscard]] constexpr Type* data() const noexcept
        {
            return dense ? dense->data() : nullptr;
        }

        [[nodiscard]] constexpr difference_type index() const noexcept
        {
            return offset - 1;
        }

    private:
        std::vector<Type>* dense;
        difference_type offset;
    };

    template<typename Type>
    [[nodiscard]] constexpr bool operator==(SparseSetIterator<Type> const& lhs, SparseSetIterator<Type> const& rhs)
    {
        return lhs.index() == rhs.index();
    }

    template<typename Type>
    [[nodiscard]] constexpr bool operator!=(SparseSetIterator<Type> const& lhs, SparseSetIterator<Type> const& rhs)
    {
        return !(lhs == rhs);
    }

    // contains only a sparse and dense array with indices pointing towards each other
    // no value type
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
            return true;
        }

    protected:
        virtual void swapAndPop(size_type denseIndex) = 0;

        std::vector<size_type> sparse; // contains indices to dense array
        std::vector<size_type> dense; // contains indices to sparse array
    };

    // implementation of SparseSetBase, contains the dense array with *values*,
    // instead of just the dense and sparse array with indices
    template<typename Type>
    class SparseSet final : public SparseSetBase
    {
    public:
        using iterator = SparseSetIterator<Type>;

        explicit SparseSet() = default;

        ~SparseSet() override = default;

        // emplace a value in the set at the given index
        // returns whether emplacing was successful
        template<typename... Args>
        bool emplace(size_type index, Args&& ... args)
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

        void swapAndPop(size_type denseIndex) override
        {
            // swap and pop dense values
            std::swap(denseValues[denseIndex], denseValues.back());
            denseValues.pop_back();
        }

        // iterators
        [[nodiscard]] iterator begin() noexcept
        {
            auto const pos = static_cast<iterator::difference_type>(denseValues.size()); // begin at the last index
            return iterator{&denseValues, pos};
        }

        [[nodiscard]] iterator end() noexcept
        {
            return iterator{&denseValues, 0}; // end at 0
        }

    private:
        std::vector<Type> denseValues; // contains values (ordered 1:1 with dense array)
    };
}

#endif //SHAPEREALITY_SPARSE_SET_H
