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
            std::cout << "direct fold expression" << std::endl;
            (..., (std::cout << static_cast<SparseSetBase&>(_components).size() << std::endl));

            std::cout << "first" << std::endl;
            test(std::index_sequence_for<Types...>());

            std::cout << "second" << std::endl;
            test(std::make_index_sequence<2>());

            std::cout << "third" << std::endl;
            test(std::make_index_sequence<1>());

            std::cout << "fourth" << std::endl;
            test(std::make_index_sequence<0>());

            std::cout << "what a thing: " << std::endl;

            size_type maxSize = 0;

            ([&_components, &maxSize] {
                auto& set = static_cast<SparseSetBase&>(_components);
                size_type size = set.denseSize();
                std::cout << "amazing size: " << size << std::endl;
                maxSize = std::max(maxSize, size);
            }(), ...);

            std::cout << "the max size was: " << maxSize << std::endl;

            auto lambda = [](SparseSetBase& input, size_type& maxSize) {
                std::cout << "damn it works: " << input.denseSize() << std::endl;
            };

            // lambda capture vs reference

            size_type maxSize2 = 0;
            (lambda(_components, maxSize2), ...);
        }


        template<size_t... Indices>
        void test(std::index_sequence<Indices...>)
        {
            (..., (std::cout << static_cast<SparseSetBase&>(std::get<Indices>(components)).denseSize() << std::endl));
        }

        ~View() = default;

    private:
        std::tuple<Types...> components;
    };
}

#endif //SHAPEREALITY_VIEW_H
