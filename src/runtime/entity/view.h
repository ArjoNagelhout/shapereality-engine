//
// Created by Arjo Nagelhout on 06/01/2024.
//

#ifndef SHAPEREALITY_VIEW_H
#define SHAPEREALITY_VIEW_H

#include <tuple>
#include <iostream>

namespace entity
{
    /**
     * A view is a
     *
     * @tparam Types which component types to get
     */
    template<typename... Types>
    requires (std::is_base_of_v<SparseSetBase, Types> && ...)
    class View final
    {
    public:
        explicit View(Types& ..._components) : components(_components...)
        {
            int minSize = 0;
            //(..., (std::cout << static_cast<SparseSetBase&>(_components).size() << std::endl));

            std::cout << "first" << std::endl;
            test(std::index_sequence_for<Types...>());

            std::cout << "second" << std::endl;
            test(std::make_index_sequence<2>());

            std::cout << "third" << std::endl;
            test(std::make_index_sequence<1>());

            std::cout << "fourth" << std::endl;
            test(std::make_index_sequence<0>());

            //auto seq = std::index_sequence_for<Types...>();

//            std::apply([this]() {
//                std::cout << components.size() << std::endl;
//            }, components);
        }



        template<size_t... Indices>
        void test(std::index_sequence<Indices...>)
        {
            (..., (std::cout << static_cast<SparseSetBase&>(std::get<Indices>(components)).denseSize() << std::endl));
        }

        ~View() = default;

    private:
        std::tuple<Types& ...> components;
    };
}

#endif //SHAPEREALITY_VIEW_H
