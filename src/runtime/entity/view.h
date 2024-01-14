//
// Created by Arjo Nagelhout on 06/01/2024.
//

#ifndef SHAPEREALITY_VIEW_H
#define SHAPEREALITY_VIEW_H

#include <tuple>
#include <iostream>

namespace entity
{
    template<typename... Types>
    class ViewIterator final
    {
    public:
        explicit ViewIterator()
        {

        }

        ~ViewIterator() = default;


    private:
        // returns whether the given index is valid
        [[nodiscard]] bool valid(entity_type entityId) const
        {
            // iterate over all pools and check if at that index all types are not TOMBSTONE

            return false;
        }
    };

    /**
     * A view enables iterating over entities that contain a set of types,
     * e.g. TransformComponent, MeshRendererComponent
     *
     * @tparam Types which component types to get
     */
    template<typename... Types>
    requires (std::is_base_of_v<SparseSetBase, Types> && ...)

    class View final
    {
    public:
        using iterator = ViewIterator<Types...>;

        explicit View(Types& ..._components) : components(&_components...)
        {
            updateView();
            std::cout << "dense size: " << view->denseSize() << std::endl;
        }

        ~View() = default;

        [[nodiscard]] iterator begin()
        {
            return iterator{};
        }

        [[nodiscard]] iterator end()
        {
            return iterator{};
        }

    private:
        std::tuple<Types* ...> components;
        SparseSetBase const* view; // the sparse set to use

        void updateView()
        {
            view = std::get<0>(components);
            std::apply([this](auto* ...other) {
                ((this->view = other->denseSize() < this->view->denseSize() ? other : this->view), ...);
            }, components);
        }
    };
}

#endif //SHAPEREALITY_VIEW_H
