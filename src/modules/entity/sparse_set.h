//
// Created by Arjo Nagelhout on 30/12/2023.
//

#ifndef SHAPEREALITY_SPARSE_SET_H
#define SHAPEREALITY_SPARSE_SET_H

#include "config.h"

#include <vector>
#include <algorithm>

#include "reflection/reflection_friend.h"

namespace entity
{
    // an "empty" value in the sparse set
    constexpr size_type kNullEntityId = std::numeric_limits<size_t>::max();

    // max size is always +1 compared to max index, but here we want to limit
    // to one less than tombstone. So + 1 - 1 cancel each other out.
    constexpr size_type kMaxSize = kNullEntityId;

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

        constexpr explicit SparseSetIterator() : dense(nullptr), offset(0)
        {}

        constexpr explicit SparseSetIterator(std::vector<Type>* dense, difference_type offset)
            : dense(dense), offset(offset)
        {}

        constexpr SparseSetIterator& operator++()
        {
            return --offset, *this;;
        }

        constexpr SparseSetIterator operator++(int)
        {
            SparseSetIterator original = *this;
            ++(*this);
            return original;
        }

        constexpr SparseSetIterator& operator--()
        {
            return ++offset, *this;
        }

        constexpr SparseSetIterator operator--(int)
        {
            SparseSetIterator original = *this;
            --(*this);
            return original;
        }

        constexpr SparseSetIterator& operator+=(difference_type const value)
        {
            offset -= value; // see note above, we use reverse iteration
            return *this;
        }

        constexpr SparseSetIterator operator+(difference_type const value) const
        {
            SparseSetIterator copy = *this;
            return (copy += value);
        }

        constexpr SparseSetIterator& operator-=(difference_type const value)
        {
            return (*this += -value);
        }

        constexpr SparseSetIterator operator-(difference_type const value) const
        {
            return (*this + -value);
        }

        // returns a pointer (standard for iterators)
        [[nodiscard]] constexpr Type* operator->()
        {
            return dense ? dense->data() + index() : nullptr;
        }

        // returns reference (standard for iterators)
        [[nodiscard]] constexpr Type& operator*()
        {
            return *operator->();
        }

        [[nodiscard]] constexpr Type* data() const
        {
            return dense ? dense->data() : nullptr;
        }

        [[nodiscard]] constexpr difference_type index() const
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
        using base_iterator = SparseSetIterator<size_type>;

        virtual ~SparseSetBase() = default;

        // returns whether the set contains an item at the given index
        [[nodiscard]] bool contains(size_type index) const
        {
            if (index >= sparse.size())
            {
                return false;
            }

            return sparse[index] != kNullEntityId;
        }

        // get the size of the sparse array
        [[nodiscard]] size_type size() const
        {
            return sparse.size();
        }

        [[nodiscard]] size_type denseSize() const
        {
            return dense.size();
        }

        // resizes the sparse array
        bool resize(size_type size)
        {
            if (size >= kMaxSize)
            {
                return false;
            }

            if (size > sparse.size())
            {
                // resize and initialize with null indices
                sparse.resize(size, kNullEntityId);
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
            sparse[index] = kNullEntityId;

            onSwapAndPop(denseIndex);
            return true;
        }

        // iterators that are used by a view
        [[nodiscard]] base_iterator beginBase()
        {
            auto const pos = static_cast<base_iterator::difference_type>(dense.size()); // begin at the last index
            return base_iterator{&dense, pos};
        }

        [[nodiscard]] base_iterator endBase()
        {
            return base_iterator{&dense, 0}; // end at 0
        }

        // clears the entire sparse set, both its sparse and dense array
        virtual void clear()
        {
            sparse.clear();
            dense.clear();
        }

    protected:
        // virtual methods that should be implemented in inherited class to also update
        // the denseValues, instead of just dense.
        virtual void onSwap(size_type lhsDenseIndex, size_type rhsDenseIndex) = 0;
        virtual void onSwapAndPop(size_type denseIndex) = 0;

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
            if ((index + 1) >= kMaxSize)
            {
                return false; // error: index out of range
            }

            if (contains(index))
            {
                return false; // error: entity at index already exists
            }

            // e.g. index = 3, size = 3. means we need to resize to size = 4
            if (index >= sparse.size())
            {
                // resize and initialize with null indices
                sparse.resize(index + 1, kNullEntityId);
            }

            dense.emplace_back(index); // set sparse index in dense array

            size_type denseIndex = dense.size() - 1;
            sparse[index] = denseIndex; // set dense index in sparse array
            denseValues.emplace_back(args...); // emplace value in dense array

            return true;
        }

        // implementation taken straight from entt and skypjack's following blog post:
        // https://skypjack.github.io/2019-09-25-ecs-baf-part-5/
        template<typename Compare, typename... Args>
        bool sort(Compare compare, Args&& ... args)
        {
            std::sort(dense.begin(), dense.end(), std::move(compare), std::forward<Args>(args)...);

            for (size_type i{}, end = dense.size(); i < end; ++i)
            {
                auto current = i;
                auto next = sparse[dense[current]];

                while (current != next)
                {
                    onSwap(dense[current], dense[next]);
                    sparse[dense[current]] = current;

                    current = next;
                    next = sparse[dense[current]];
                }
            }

            return false;
        }

        Type& get(size_type index)
        {
            return denseValues[sparse[index]];
        }

        // iterators, these enable range-based for loops
        [[nodiscard]] iterator begin()
        {
            auto const pos = static_cast<iterator::difference_type>(denseValues.size()); // begin at the last index
            return iterator{&denseValues, pos};
        }

        [[nodiscard]] iterator end()
        {
            return iterator{&denseValues, 0}; // end at 0
        }

        void clear() override
        {
            SparseSetBase::clear();
            denseValues.clear();
        }

    protected:
        void onSwap(entity::size_type lhsDenseIndex, entity::size_type rhsDenseIndex) override
        {
            std::swap(denseValues[sparse[lhsDenseIndex]], denseValues[sparse[rhsDenseIndex]]);
        }
        
        void onSwapAndPop(size_type denseIndex) override
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
