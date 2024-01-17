//
// Created by Arjo Nagelhout on 06/01/2024.
//

#ifndef SHAPEREALITY_VIEW_H
#define SHAPEREALITY_VIEW_H

#include <tuple>
#include <iostream>

namespace entity
{
    /*
     * ViewIterator wraps an iterator of the view property of View
     */
    template<typename... Types>
    class ViewIterator final
    {
    public:
        using iterator = SparseSetBase::base_iterator;

        explicit ViewIterator() : current{}, last{}
        {}

        explicit ViewIterator(iterator current, iterator last, std::tuple<Types* ...> _components)
            : current(current), last(last), components(_components)
        {
        }

        ViewIterator& operator++()
        {
            while (++current != last && !valid(*current))
            {}
            return *this;
        }

        ViewIterator operator++(int)
        {
            ViewIterator orig = *this;
            ++(*this);
            return orig;
        }

        // return a tuple containing an rvalue reference to each component in the view
        // at the current entityId
        [[nodiscard]] decltype(auto) operator*()
        {
            auto const entityId = *current;

            return std::apply([entityId](auto* ...component) {
                return std::tie(component->get(entityId)...);
            }, components);
        }

        ~ViewIterator() = default;

    private:
        iterator current;
        iterator last;
        std::tuple<Types* ...> components;

        // returns whether the given index is valid
        [[nodiscard]] bool valid(entity_type entityId) const
        {
            if (entityId == TOMBSTONE)
            {
                return false;
            }

            // iterate over all components to check if they contain the provided entityId
            bool invalid = false;
            std::apply([&entityId, &invalid](auto* ...component) {
                ((invalid = !component->contains(entityId) || invalid), ...);
            }, components);

            return !invalid;
        }

        template<typename... TypesLhs, typename... TypesRhs>
        friend constexpr bool operator==(ViewIterator<TypesLhs...> const&, ViewIterator<TypesRhs...> const&);
    };

    template<typename... TypesLhs, typename... TypesRhs>
    [[nodiscard]] constexpr bool operator==(ViewIterator<TypesLhs...> const& lhs, ViewIterator<TypesRhs...> const& rhs)
    {
        return lhs.current == rhs.current;
    }

    template<typename... TypesLhs, typename... TypesRhs>
    [[nodiscard]] constexpr bool operator!=(ViewIterator<TypesLhs...> const& lhs, ViewIterator<TypesRhs...> const& rhs)
    {
        return !(lhs == rhs);
    }

    enum class IterationPolicy
    {
        /*
         * default:
         * use the component type with the smallest dense size (least amount of valid entities)
         */
        UseSmallestComponent,

        /*
         * use the first component type in the variadic template arguments
         *
         * e.g. r.view<MeshRendererComponent,
         *             HierarchyComponent,
         *             TransformComponent>(IterationPolicy::UseFirstComponent);
         *
         * always iterates using the order of the dense array of MeshRendererComponent
         */
        UseFirstComponent,
    };

    /**
     * A view enables iterating over entities that contain a set of types,
     * e.g. TransformComponent, MeshRendererComponent
     *
     * @tparam Types which component types to get
     */
    template<typename... Types>
    requires (std::is_base_of_v<SparseSetBase, Types> && ...) // fold expression
    && (sizeof...(Types) > 1u) // why would you ever make a view of a single type, just directly iterate over the sparse set
    class View final
    {
    public:
        using iterator = ViewIterator<Types...>;

        explicit View(IterationPolicy _iterationPolicy, Types* ..._components) :
            iterationPolicy(_iterationPolicy), components(_components...)
        {
            updateView();
        }

        ~View() = default;

        [[nodiscard]] iterator begin()
        {
            return view ? iterator{view->beginBase(), view->endBase(), components} : iterator{};
        }

        [[nodiscard]] iterator end()
        {
            return view ? iterator{view->endBase(), view->endBase(), components} : iterator{};
        }

        // returns the maximum size of the view that will be iterated on
        //
        // the actual size can turn out to be less, as the overlap between the component type with the least
        // entries and all other component types could be anywhere between the size of the component type with
        // the least entries, and 0.
        [[nodiscard]] size_type maxSize()
        {
            return view ? view->denseSize() : 0;
        }

    private:
        std::tuple<Types* ...> components;
        SparseSetBase* view{nullptr}; // the sparse set to use for iteration
        IterationPolicy iterationPolicy;

        void updateView()
        {
            switch (iterationPolicy)
            {
                case IterationPolicy::UseFirstComponent:view = std::get<0>(components);
                    break;
                case IterationPolicy::UseSmallestComponent:
                    // select the component type with the smallest denseSize
                    view = std::get<0>(components);
                    std::apply([&_view = view](auto* ...component) {
                        ((_view = (component ? component->denseSize() : 0) < _view->denseSize() ? component : _view), ...);
                    }, components);
                    break;
            }
        }
    };
}

#endif //SHAPEREALITY_VIEW_H
